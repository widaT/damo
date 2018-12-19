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
        if (x.distance() == y.distance()) {
            return x.name() > y.name();
        }
        return x.distance() > y.distance();
    }

    DB::DB(const string &path):pool(10){
        wo = rocksdb::WriteOptions();
        ro =rocksdb::ReadOptions();
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, path, &db);
        if (!status.ok()) {
            cerr << "rocksdb open error!" << endl;
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
        mut.lock();
        if (groupMap.find(group) == groupMap.end()) {
            groupMap[group] = 1;
            status = db->Put(wo, GROUP_PREFIX + group, to_string(1));
            if (!status.ok()) {
                mut.unlock();
                return -1;
            }
        }
        mut.unlock();
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
        rocksdb::ReadOptions read_options;
        read_options.prefix_same_as_start = true;
        auto iter = db->NewIterator(read_options);
        float ifeautre[FEATURE_SIZE] = {0};
        int num = 0;

        string grpup_prefix = group + SPLIT_STR;
        for (iter->Seek(grpup_prefix); iter->Valid(); iter->Next()) {
            auto sfeature = iter->value().ToString();
            unpack(ifeautre, sfeature);
            auto distance = avx_euclidean_distance(ifeautre, feature);
            auto key = iter->key();
            key.remove_prefix(grpup_prefix.size());
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
            sort(users.rbegin(), users.rend(), cmp);
            num++;
        }
        mut.lock();
        if (groupMap.find(group) != groupMap.end()) { //group 有可能这个时候被删除
            groupMap[group] = num;
        }
        mut.unlock();
        return 0;
    }

    int DB::QSearch(const string &group, float *feature, vector<pb::SearchReply_User> &users) {
        static const vector<tuple<string, string>> keys = {
                make_tuple("0", "3"),//左开右闭
                make_tuple("3", "6"),
                make_tuple("6", "a"),
                make_tuple("a", "e"),
                make_tuple("e", "{")};

        std::vector<std::future<int>> rets;
        vector<SearchReply_User> user_arr[5];
        int i = 0;
        for (auto &range:keys) {
            rets.emplace_back(
                    pool.enqueue([&, i] {
                        return _search(group, get<0>(range), get<1>(range), feature, user_arr[i]);
                    })
            );
            i++;
        }
        int total = 0;
        for (auto &&ret:rets) {
            total += ret.get();
        }
        for (auto const &user : user_arr) {
            users.insert(users.end(), user.begin(), user.end());
        }
        mut.lock();
        if (groupMap.find(group) != groupMap.end()) { //group 有可能这个时候被删除
            groupMap[group] = total;
        }
        mut.unlock();
        sort(users.rbegin(), users.rend(), cmp);
        if (users.size() > 5) {
            users.erase(users.begin() + 5, users.end());
        }
        return 0;
    }


    int DB::_search(const string &group, const string &startkey, const string &endkey,
                    float *feature, vector<SearchReply_User> &users) {
        int num = 0;
        rocksdb::ReadOptions read_options;
        read_options.prefix_same_as_start = true;
        auto iter = db->NewIterator(read_options);
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
            sort(users.rbegin(), users.rend(), cmp);
            num++;
        }
        return num;
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
        mut.lock();
        size_t ret = groupMap.erase(group);
        mut.unlock();
        //@todo 非线程安全 需要改进
        if (ret == 1) {
            string start(group + SPLIT_STR);
            string end(group + SPLIT_STR +"{");
            rocksdb::WriteBatch batch;
            batch.Delete(GROUP_PREFIX + group);
            batch.DeleteRange(start,end);
            auto status = db->Write(wo,& batch);
            if (!status.ok()) {
                return -1;
            }
        }
        return 0;
    }

    int DB::GroupList(vector<string> &groups) {
        mut.lock();
        auto iter = groupMap.begin();
        while (iter != groupMap.end()) {
            groups.push_back(iter->first);
            iter++;
        }
        mut.unlock();
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
        mut.lock();
        uint64_t size = 0;
        if (groupMap.find(group) != groupMap.end()) {
            size = uint64_t(groupMap[group]);
        }
        /* if (size == 1) {  //为1的时候可能是没有search过，利用rocksdb的文件和内存占用大小预估groupsize
             rocksdb::Range range[1];
             range[0] = rocksdb::Range(group + SPLIT_STR, group + SPLIT_STR + "{");
             uint64_t size[1];
             db->GetApproximateSizes(range, 1, size, true);
             size  =size[0]/2048;
        }*/
        mut.unlock();
        return size;
    }

    int DB::Info(pb::InfoReply &infoReply) {
        mut.lock();
        infoReply.set_groupslen(int(groupMap.size()));
        for (auto const &v:groupMap) {
            pb::InfoReply_GroupInfo *group = infoReply.add_groups();
            group->set_name(v.first);
            group->set_len(v.second);
        }
        mut.unlock();
    }

    DB::~DB() {
        db->Close();
    }
}
