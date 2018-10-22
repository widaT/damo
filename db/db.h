//
// Created by wida on 18-10-16.
//

#ifndef TEST_DB_H
#define TEST_DB_H

#include <rocksdb/db.h>
#include "string"
#include "../pb/search.pb.h"

namespace db {
    class DB {
    private:
        rocksdb::DB *db;
        rocksdb::WriteOptions wo = rocksdb::WriteOptions();
        rocksdb::ReadOptions ro = rocksdb::ReadOptions();
    public:
        DB(std::string);

        int Put(std::string, std::string, float *feature);

        int Get(std::string key, std::string id, float *feautre);

        int Delete(std::string, std::string);

        int Search(std::string, float *, std::vector<pb::SearchReply_User> &);

        ~DB();
    };
}
#endif //TEST_DB_H
