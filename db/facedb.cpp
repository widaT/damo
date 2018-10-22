//
// Created by wida on 18-10-19.
//

#include "facedb.h"

using namespace std;
namespace db {
    Facedb *Facedb::instance = new Facedb();
    DB *db = new DB("/tmp/testdb");
    Facedb *Facedb::getInstance() {
        return Facedb::instance;
    }

    Facedb::Facedb() {}

    int Facedb::addUser(string group,string id, float * feature) {
        return db->Put(group,id,feature);
    }

    int Facedb::search(string group,float * ffeature,vector<pb::SearchReply_User> & users) {
        db->Search(group,ffeature,users);
        return 0;
    }

    Facedb::~Facedb() {
        delete Facedb::instance;
        delete db;
    };
}