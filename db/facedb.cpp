//
// Created by wida on 18-10-19.
//

#include "facedb.h"


Facedb* Facedb::getInstance() {
     return instance;
}

Facedb::Facedb() {
    rocksdb::Options options;
    options.create_if_missing = true;
    instance = new Facedb();
    *db = DB(options,"/tmp/testdb");
}

Facedb::~Facedb() {
   delete instance;
};