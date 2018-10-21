//
// Created by wida on 18-10-16.
//

#ifndef TEST_DB_H
#define TEST_DB_H
#include <rocksdb/db.h>
#include "string"
namespace db {
    class DB {
    private:
        rocksdb::DB *db;
        rocksdb::WriteOptions wo = rocksdb::WriteOptions();
        rocksdb::ReadOptions ro = rocksdb::ReadOptions();
    public:
        DB(std::string);
        int Put(std::string, std::string);
        int Get(std::string, std::string *);
        int Delete(std::string);
        ~DB();
    };
}
#endif //TEST_DB_H
