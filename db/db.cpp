//
// Created by wida on 18-10-16.
//
#include "db.h"
#include "common.h"


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

    DB::~DB() {
        db->Close();
    }
}
