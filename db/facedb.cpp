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
    *db = DB::DB(options,"/tmp/testdb");
}

Facedb::~Facedb() {
   delete db;
   delete instance;
};