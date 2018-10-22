//
// Created by wida on 18-10-16.
//
#include "db.h"
#include <sstream>
#include <immintrin.h>
#define FEATURE_SIZE 512
using namespace std;
using namespace pb;
namespace db {
    const string SPLIT_STR="--+++--";
    float avx_euclidean_distance( float *x, float *y)
    {
        static const size_t single_size = 8;
        const size_t end = FEATURE_SIZE / single_size;
        __m256 *vx = (__m256 *)x;
        __m256 *vy = (__m256 *)y;
        __m256 vsub = {0};
        __m256 vsum = {0};
        for(size_t i=0; i<end; ++i) {
            vsub = _mm256_sub_ps(vx[i], vy[i]);
            vsum = _mm256_add_ps(vsum, _mm256_mul_ps(vsub, vsub));
        }
        __attribute__((aligned(32))) float t[8] = {0};
        _mm256_store_ps(t, vsum);
        return t[0] + t[1] + t[2] + t[3] + t[4] + t[5] + t[6] + t[7];
    }

    float distance(float *a,float *b) {
        float distance  = 0.0;
        for (int i =0;i<FEATURE_SIZE;i++) {
            float sub = *(a+i) - *(b+i);
            distance += sub * sub;
        };
        return distance;
    }


    void pack(float *ffeature,string & sfeature){
        ostringstream f;
        f.write((char*)ffeature, FEATURE_SIZE*sizeof(float));      //fwrite以char *的方式进行写出，做一个转化
        sfeature = f.str();
        // f.clear();
    };

    void unpack(float *ffeature,string & sfeature) {
        istringstream r = istringstream(sfeature);
        r.read((char *)ffeature,FEATURE_SIZE*sizeof(float));
        //r.clear();
    }


    bool cmp(SearchReply_User x,SearchReply_User y) {
        return x.distance()<y.distance();
    }

    DB::DB(string path) {
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, path ,&db);
        assert(status.ok());
    }

    int DB::Put(string group, string id,float * feature) {
        string sfeature;
        pack(feature,sfeature);
        rocksdb::Status status = db->Put(wo, group+SPLIT_STR+id, sfeature);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::Get(string group, string id,float * ifeautre) {
        string sfeature;
        rocksdb::Status status = db->Get(ro, group+SPLIT_STR+id, &sfeature);
        if (!status.ok()) {
            return -1;
        }
        unpack(ifeautre,sfeature);
        return 0;
    }

    int DB::Delete(string key) {
        rocksdb::Status status =  db->Delete(wo, key);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::Search(string group,float * feature,vector<pb::SearchReply_User> &users) {
        auto iter = db->NewIterator(ro);
        float ifeautre[FEATURE_SIZE] = {0};
        for (iter->Seek(group);iter->Valid() && iter->key().starts_with(group);iter->Next()){
            auto sfeature = iter->value().ToString();
            unpack(ifeautre,sfeature);
            auto distance = avx_euclidean_distance(ifeautre, feature);
            auto key = iter->key();
            key.remove_prefix((group+SPLIT_STR).size());
            if (users.size() < 5) {
                pb::SearchReply_User user;
                user.set_name(key.data());
                user.set_distance(distance);
                users.push_back(user);
            }else {
                 if (users.end()->distance() > distance ) {
                     users.end()->set_distance(distance);
                     users.end()->set_name(key.data());
                }
            }
            sort(users.begin(),users.end(),cmp);
        }
    }

    DB::~DB() {
        db->Close();
    }
}
