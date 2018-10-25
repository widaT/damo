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

    DB::DB(string path){
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, path, &db);
        assert(status.ok());
        auto iter = db->NewIterator(ro);
        for (iter->Seek(GROUP_PREFIX); iter->Valid() && iter->key().starts_with(GROUP_PREFIX); iter->Next()) {
            auto key = iter->key();
            key.remove_prefix(string(GROUP_PREFIX).size());
            groupMap[key.ToString()] =  atoi(iter->value().data());
        }
    }

    int DB::Put(string group, string id, float *feature) {
        rocksdb::Status status;
        mutex.lock();
        if (groupMap.find(group) == groupMap.end()){
            groupMap[group] = 1;
            status = db->Put(wo,GROUP_PREFIX+group,to_string(1));
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

    int DB::Get(string group, string id, float *ifeautre) {
        string sfeature;
        rocksdb::Status status = db->Get(ro, group + SPLIT_STR + id, &sfeature);
        if (!status.ok()) {
            return -1;
        }
        unpack(ifeautre, sfeature);
        return 0;
    }

    int DB::Search(string group, float *feature, vector<pb::SearchReply_User> &users) {
        if(GroupSize(group) > NEED_MTREAD_SEARCH ) {
            return QSearch(group,feature,users);
        }else {
            return NSearch(group,feature,users);
        }
    }

    int DB::NSearch(string group, float *feature, vector<pb::SearchReply_User> &users) {
        auto iter = db->NewIterator(ro);
        float ifeautre[FEATURE_SIZE] = {0};
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
                if (users.end()->distance() > distance) {
                    users.end()->set_distance(distance);
                    users.end()->set_name(key.ToString());
                }
            }
            sort(users.begin(), users.end(), cmp);
        }
        return 0;
    }

    int DB::QSearch(string group, float *feature, vector<pb::SearchReply_User> &users) {
        static const vector<tuple<string,string>> keys = {
                make_tuple("0","a"),//左开右闭
                make_tuple("a","h"),
                make_tuple("h","0"),
                make_tuple("o","u"),
                make_tuple("u","{")};

        vector<SearchReply_User> user_arr[5] ;
        vector<thread> threads;
        int i =0;

            for (auto &range:keys) {
                //多线程 lambda 引用捕获外部变量
                threads.emplace_back([&](){
                    _search(group,get<0>(range),get<1>(range),feature,user_arr[i++]);
                });
            }
            for (auto &tr:threads) {
                tr.join();
            }


        for (auto &u :user_arr){
            users.insert(users.end(),u.begin(),u.end());
        }

        sort(users.begin(), users.end(), cmp);
        if (users.size() > 5) {
            users.erase(users.begin()+5,users.end());
        }
        return 0;
    }


    void DB::_search(string group, string startkey,string endkey, float *feature, vector<SearchReply_User> &users) {
        auto iter = db->NewIterator(ro);
        float ifeautre[FEATURE_SIZE];
        string skey = group + SPLIT_STR + startkey;
        string ekey = group + SPLIT_STR + endkey;

        cout << skey << ekey << endl;
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
             sort(users.begin(),users.end(), cmp);
        }
    }

    int DB::Delete(string group, string id) {
        group = group + SPLIT_STR + id;
        rocksdb::Status status = db->Delete(wo, group);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::DelGroup(string group) {
        mutex.lock();
        size_t ret = groupMap.erase(group);
        mutex.unlock();
        //@todo 非线程安全 需要改进
        if (ret == 1) {
            rocksdb::Status status = db->Delete(wo,GROUP_PREFIX+group);
            if (!status.ok()) {
                return -1;
            }
            //删除用户
            auto iter = db->NewIterator(ro);
            string prefix(group + SPLIT_STR);
            for (iter->Seek(prefix); iter->Valid() && iter->key().starts_with(prefix); iter->Next()) {
                db->Delete(wo,iter->key());
            }
        }
        return 0;
    }

    int DB::GroupList(vector<string> & groups) {
        mutex.lock();
        auto iter = groupMap.begin();
        while(iter != groupMap.end()){
            groups.push_back(iter->first);
            iter++;
        }
        mutex.unlock();
        return 0;
    }

    int DB::UserList(string group, string startkey, int num,vector<string> & users) {
        auto iter = db->NewIterator(ro);
        int n = 0;
        string prifex(group+SPLIT_STR);
        for (iter->Seek(prifex + startkey); iter->Valid() && iter->key().starts_with(prifex) && n <num; iter->Next()) {
            auto key = iter->key();
            key.remove_prefix(prifex.size());
            users.emplace_back(key.ToString());
            n++;
        }
        return 0;
    }


    uint64_t DB::GroupSize(string group) {
        rocksdb::Range range[1];
        range[0]= rocksdb::Range(group+SPLIT_STR,group+SPLIT_STR+"{");
        uint64_t size[1];
        db->GetApproximateSizes(range,1,size, true);
        return size[0];
    }

    DB::~DB() {
        db->Close();
    }
}
