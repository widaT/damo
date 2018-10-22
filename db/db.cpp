//
// Created by wida on 18-10-16.
//
#include "db.h"
#include "common.h"

using namespace std;
using namespace pb;
namespace db {
    const string SPLIT_STR = "--+++--";


    bool cmp(SearchReply_User x, SearchReply_User y) {
        return x.distance() < y.distance();
    }

    DB::DB(string path) {
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, path, &db);
        assert(status.ok());
    }

    int DB::Put(string group, string id, float *feature) {
        string sfeature;
        pack(feature, sfeature);
        rocksdb::Status status = db->Put(wo, group + SPLIT_STR + id, sfeature);
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
                user.set_name(key.data());
                user.set_distance(distance);
                users.push_back(user);

            } else {
                if (users.end()->distance() > distance) {
                    users.end()->set_distance(distance);
                    users.end()->set_name(key.data());
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

    DB::~DB() {
        db->Close();
    }
}
