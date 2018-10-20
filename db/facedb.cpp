//
// Created by wida on 18-10-19.
//

#include "facedb.h"
#include "feature.pb.h"

Facedb* Facedb::getInstance() {
     return instance;
}

Facedb::Facedb() {
    rocksdb::Options options;
    options.create_if_missing = true;
    instance = new Facedb();
    *db = DB(options,"/tmp/testdb");
}


int Facedb::addUser(User user) {
    std::string featureStr;
    user.feature.SerializeToString(&featureStr);
    google::protobuf::ShutdownProtobufLibrary();
    return Facedb::db->Put(user.id,featureStr);
}


Facedb::~Facedb() {
   delete instance;
};


