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
        static Facedb *getInstance();

        int search(std::string, float *, std::vector<pb::SearchReply_User> &);

        int addUser(std::string, std::string, float *);

        int getUser(std::string, std::string, float *);

        int delUser(std::string, std::string);

        int groupList(std::vector<std::string> &);

        int userList(std::string,std::string,int num,std::vector<std::string>&);

        int delgroup(std::string);

        ~Facedb();
    };

}
#endif //DAMO_FACEDB_H
