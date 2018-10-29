//
// Created by wida on 18-10-16.
//
#include "db.h"
#include "common.h"
#include <thread>

using namespace std;
using namespace pb;

namespace db {
    bool cmp(SearchReply_User x, SearchReply_User y) {
        return x.distance() < y.distance();
    }

    DB::DB(const string &path) {
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, path, &db);
        if (!status.ok()) {
            cerr << "rocksdb open error!";
            exit(-1);
        }
        auto iter = db->NewIterator(ro);
        for (iter->Seek(GROUP_PREFIX); iter->Valid() && iter->key().starts_with(GROUP_PREFIX); iter->Next()) {
            auto key = iter->key();
            key.remove_prefix(string(GROUP_PREFIX).size());
            groupMap[key.ToString()] = atoi(iter->value().data());
        }
    }

    int DB::Put(const string &group, const string &id, float *feature) {
        rocksdb::Status status;
        mutex.lock();
        if (groupMap.find(group) == groupMap.end()) {
            groupMap[group] = 1;
            status = db->Put(wo, GROUP_PREFIX + group, to_string(1));
            if (!status.ok()) {
                mutex.unlock();
                return -1;
            }
        }
        mutex.unlock();
        string sfeature;
        pack(feature, sfeature);
        status = db->Put(wo, group + SPLIT_STR + id, sfeature);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::Get(const string &group, const string &id, float *ifeautre) {
        string sfeature;
        rocksdb::Status status = db->Get(ro, group + SPLIT_STR + id, &sfeature);
        if (!status.ok()) {
            return -1;
        }
        unpack(ifeautre, sfeature);
        return 0;
    }

    int DB::Search(const string &group, float *feature, vector<pb::SearchReply_User> &users) {
        if (GroupSize(group) > NEED_MTREAD_SEARCH) {
            return QSearch(group, feature, users);
        } else {
            return NSearch(group, feature, users);
        }
    }

    int DB::NSearch(const string &group, float *feature, vector<pb::SearchReply_User> &users) {
        auto iter = db->NewIterator(ro);
        float ifeautre[FEATURE_SIZE] = {0};
        int num = 0;
        for (iter->Seek(group); iter->Valid() && iter->key().starts_with(group); iter->Next()) {
            auto sfeature = iter->value().ToString();
            unpack(ifeautre, sfeature);
            auto distance = avx_euclidean_distance(ifeautre, feature);
            auto key = iter->key();
            key.remove_prefix((group + SPLIT_STR).size());
            if (users.size() < 5) {
                SearchReply_User user;
                user.set_name(key.ToString());
                user.set_distance(distance);
                users.push_back(user);

            } else {
                if (users[4].distance() > distance) {
                    users[4].set_distance(distance);
                    users[4].set_name(key.ToString());
                }
            }
            sort(users.begin(), users.end(), cmp);
            num++;
        }
        mutex.lock();
        if (groupMap.find(group) != groupMap.end()){ //group 有可能这个时候被删除
            groupMap[group] = num;
        }
        mutex.unlock();
        return 0;
    }

    int DB::QSearch(const string &group, float *feature, vector<pb::SearchReply_User> &users) {
        static const vector<tuple<string, string>> keys = {
/*                make_tuple("0", "a"),//左开右闭
                make_tuple("a", "h"),
                make_tuple("h", "0"),
                make_tuple("o", "u"),
                make_tuple("u", "{")};*/
        make_tuple("0", "3"),//左开右闭
                make_tuple("3", "6"),
                make_tuple("6", "a"),
                make_tuple("a", "e"),
                make_tuple("e", "{")};

        vector<SearchReply_User> user_arr[5];
        vector<thread> threads;
        int nums[5] = {0};
        int i = 0;
        for (auto &range:keys) {
            //多线程 lambda 引用捕获外部变量,i必须要要copy捕获
            threads.emplace_back([&,i]() {
                _search(group, get<0>(range), get<1>(range), feature, user_arr[i],nums[i]);
            });
            i++;
        }
        for (auto &tr:threads) {
            tr.join();
        }
        int totol = 0;
        for (int i=0;i< 5;i++) {
            users.insert(users.end(), user_arr[i].begin(), user_arr[i].end());
            totol += nums[i];
        }
        mutex.lock();
        if (groupMap.find(group) != groupMap.end()){ //group 有可能这个时候被删除
            groupMap[group] = totol;
        }
        mutex.unlock();
        sort(users.begin(), users.end(), cmp);
        if (users.size() > 5) {
            users.erase(users.begin() + 5, users.end());
        }
        return 0;
    }


    void DB::_search(const string &group, const string &startkey, const string &endkey,
                                float *feature, vector<SearchReply_User> &users,int &num) {
        auto iter = db->NewIterator(ro);
        float ifeautre[FEATURE_SIZE];
        string skey = group + SPLIT_STR + startkey;
        string ekey = group + SPLIT_STR + endkey;
        for (iter->Seek(skey); iter->Valid() && iter->key().compare(ekey) < 0; iter->Next()) {
            auto sfeature = iter->value().ToString();
            unpack(ifeautre, sfeature);
             auto distance = avx_euclidean_distance(ifeautre, feature);
             auto key = iter->key();
             key.remove_prefix((group + SPLIT_STR).size());
               if (users.size() < 5) {
                   SearchReply_User user;
                   user.set_name(key.ToString());
                   user.set_distance(distance);
                   users.push_back(user);
               } else {
                   if (users[4].distance() > distance) {
                       users[4].set_distance(distance);
                       users[4].set_name(key.ToString());
                   }
               }
             sort(users.begin(), users.end(), cmp);
             num ++;
        }
    }

    int DB::Delete(const string &group, const string &id) {
        string sgroup = group + SPLIT_STR + id;
        rocksdb::Status status = db->Delete(wo, sgroup);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::DelGroup(const string &group) {
        mutex.lock();
        size_t ret = groupMap.erase(group);
        mutex.unlock();
        //@todo 非线程安全 需要改进
        if (ret == 1) {
            rocksdb::Status status = db->Delete(wo, GROUP_PREFIX + group);
            if (!status.ok()) {
                return -1;
            }
            //删除用户
            auto iter = db->NewIterator(ro);
            string prefix(group + SPLIT_STR);
            for (iter->Seek(prefix); iter->Valid() && iter->key().starts_with(prefix); iter->Next()) {
                db->Delete(wo, iter->key());
            }
        }
        return 0;
    }

    int DB::GroupList(vector<string> &groups) {
        mutex.lock();
        auto iter = groupMap.begin();
        while (iter != groupMap.end()) {
            groups.push_back(iter->first);
            iter++;
        }
        mutex.unlock();
        return 0;
    }

    int DB::UserList(const string &group, const string &startkey, int num, vector<string> &users) {
        auto iter = db->NewIterator(ro);
        int n = 0;
        string prifex(group + SPLIT_STR);
        for (iter->Seek(prifex + startkey); iter->Valid() && iter->key().starts_with(prifex) && n < num; iter->Next()) {
            auto key = iter->key();
            key.remove_prefix(prifex.size());
            users.emplace_back(key.ToString());
            n++;
        }
        return 0;
    }


    uint64_t DB::GroupSize(const string &group) {
        mutex.lock();
        uint64_t  size = 0;
        if (groupMap.find(group) != groupMap.end()) {
            size  = uint64_t(groupMap[group]);
        }
        /* if (size == 1) {  //为1的时候可能是没有search过，利用rocksdb的文件和内存占用大小预估groupsize
             rocksdb::Range range[1];
             range[0] = rocksdb::Range(group + SPLIT_STR, group + SPLIT_STR + "{");
             uint64_t size[1];
             db->GetApproximateSizes(range, 1, size, true);
             size  =size[0]/2048;
        }*/
        mutex.unlock();
        return size;
    }

    int DB::Info(pb::InfoReply & infoReply) {
        mutex.lock();
        infoReply.set_groupslen(int(groupMap.size()));
        for(auto const &v:groupMap) {
            pb::InfoReply_GroupInfo* group = infoReply.add_groups();
            group->set_name(v.first);
            group->set_len(v.second);
        }
        mutex.unlock();
    }

    DB::~DB() {
        db->Close();
    }
}
