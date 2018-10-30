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
        const uint64_t NEED_MTREAD_SEARCH = 200000; //20w 就采用的多线程search

    private:
        rocksdb::DB *db;
        rocksdb::WriteOptions wo = rocksdb::WriteOptions();
        rocksdb::ReadOptions ro = rocksdb::ReadOptions();
        std::map<std::string,int> groupMap;
        std::mutex  mut;

    public:
        DB(const std::string&);

        int Put(const std::string &, const std::string &, float *feature);

        int Get(const std::string &, const std::string &, float *feautre);

        int Delete(const std::string &, const std::string &);

        int Search(const std::string &, float *, std::vector<pb::SearchReply_User> &);

        int NSearch(const std::string &, float *, std::vector<pb::SearchReply_User> &);//单线程

        int QSearch(const std::string &, float *, std::vector<pb::SearchReply_User> &);//多线程

        void _search(const std::string &, const std::string &,const std::string  &, float *, std::vector<pb::SearchReply_User> &,int&);

        int DelGroup(const std::string &);

        int GroupList(std::vector<std::string> &);

        int UserList(const std::string & ,const std::string & ,int,std::vector<std::string>&);

        uint64_t GroupSize(const std::string &);

        int Info(pb::InfoReply &);

        ~DB();
    };
}
#endif //TEST_DB_H
