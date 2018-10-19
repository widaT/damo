//
// Created by wida on 18-10-16.
//
#include "db.h"
namespace db {
    DB::DB(rocksdb::Options options , std::string path) {
        rocksdb::Status status = rocksdb::DB::Open(options, path ,&db);
        assert(status.ok());
    }

    int DB::Put(std::string key, std::string val) {
        rocksdb::Status status = db->Put(wo, key, val);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::Get(std::string key, std::string *val) {
        rocksdb::Status status = db->Get(ro, key, val);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    int DB::Delete(std::string key) {
        rocksdb::Status status =  db->Delete(wo, key);
        if (!status.ok()) {
            return -1;
        }
        return 0;
    }

    DB::~DB() {
        db->Close();
    }
}
