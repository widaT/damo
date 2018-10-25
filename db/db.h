//
// Created by wida on 18-10-16.
//

#ifndef TEST_DB_H
#define TEST_DB_H

#include <rocksdb/db.h>
#include "string"
#include "../pb/search.pb.h"
#include <map>
namespace db {

    class DB {
        const std::string SPLIT_STR = "--+++--";
        const std::string GROUP_PREFIX = "GROUP___";
        const uint64_t NEED_MTREAD_SEARCH = 500003290;

    private:
        rocksdb::DB *db;
        rocksdb::WriteOptions wo = rocksdb::WriteOptions();
        rocksdb::ReadOptions ro = rocksdb::ReadOptions();
        std::map<std::string,int> groupMap;
        std::mutex  mutex;

    public:
        DB(std::string);

        int Put(std::string, std::string, float *feature);

        int Get(std::string key, std::string id, float *feautre);

        int Delete(std::string, std::string);

        int Search(std::string, float *, std::vector<pb::SearchReply_User> &);

        int NSearch(std::string, float *, std::vector<pb::SearchReply_User> &);//单线程

        int QSearch(std::string, float *, std::vector<pb::SearchReply_User> &);//多线程

        void _search(std::string, std::string ,std::string , float *, std::vector<pb::SearchReply_User> &users);

        int DelGroup(std::string);

        int GroupList(std::vector<std::string> &);

        int UserList(std::string ,std::string ,int,std::vector<std::string>&);

        uint64_t GroupSize(std::string);

        ~DB();
    };
}
#endif //TEST_DB_H
