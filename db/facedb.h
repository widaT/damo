//
// Created by wida on 18-10-19.
//

#ifndef DAMO_FACEDB_H
#define DAMO_FACEDB_H

#include <string>
#include "db.h"
#include "../pb/search.pb.h"

namespace db{
    struct User  {
        std::string id;
        float *feature;
    };

    class Facedb {
    private:
        Facedb();
        //把复制构造函数和=操作符也设为私有
        Facedb(const Facedb&);
        Facedb& operator=(const Facedb&);
        static Facedb* instance;

    public:
        static Facedb* getInstance();
        int search(std::string ,std::string, float *);
        int addUser(User);
        ~Facedb();
    };

}
#endif //DAMO_FACEDB_H
