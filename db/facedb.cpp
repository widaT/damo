//
// Created by wida on 18-10-19.
//

#include "facedb.h"
#include "ini.h"
using namespace std;
using namespace config;
namespace db {

    Facedb *Facedb::instance = new Facedb();

    DB *db = new DB(INI::getInstance()->read("db","path"));

    Facedb *Facedb::getInstance() {
        return Facedb::instance;
    }

    Facedb::Facedb() {}

    int Facedb::addUser(string group, string id, float *feature) {
        return db->Put(group, id, feature);
    }

    int Facedb::getUser(string group, string id, float *feature) {
        return db->Get(group, id, feature);
    }

    int Facedb::search(string group, float *ffeature, vector<pb::SearchReply_User> &users) {
        return db->Search(group, ffeature, users);
    }

    int Facedb::delUser(std::string group, std::string id) {
        return db->Delete(group, id);
    }

    int Facedb::delgroup(std::string group) {
        return db->DelGroup(group);
    }

    int Facedb::groupList(std::vector<string> & groups) {
        return db->GroupList(groups);
    }

    int Facedb::userList(std::string group, std::string startKey, int num, std::vector<std::string> & users) {
        return db->UserList(group,startKey,num,users);
    }

    Facedb::~Facedb() {
        delete Facedb::instance;
        delete db;
    }
}