//
// Created by wida on 18-10-19.
//

#ifndef DAMO_FACEDB_H
#define DAMO_FACEDB_H

#include <string>
#include "db.h"
#include "../pb/search.pb.h"

namespace db {
    class Facedb {
    private:
        Facedb();

        //把复制构造函数和=操作符也设为私有
        Facedb(const Facedb &);

        Facedb &operator=(const Facedb &);

        static Facedb *instance;

    public:
        static Facedb *GetInstance();

        int Search(const std::string &, float *, std::vector<pb::SearchReply_User> &);

        int AddUser(const std::string &, const std::string &, float *);

        int GetUser(const std::string &, const std::string &, float *);

        int DelUser(const std::string &, const std::string &);

        int GroupList(std::vector<std::string> &);

        int UserList(const std::string &,const std::string &,int num,std::vector<std::string>&);

        int Delgroup(const std::string &);

        int Info(pb::InfoReply &);

        uint64_t GroupSize(const std::string &);

        ~Facedb();
    };

}
#endif //DAMO_FACEDB_H
