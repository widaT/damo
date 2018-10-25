//
// Created by wida on 18-10-19.
//

#include "facedb.h"
#include "ini.h"
using namespace std;
using namespace config;
namespace db {

    Facedb *Facedb::instance = new Facedb();

    DB *db = new DB(INI::GetInstance()->Read("db","path"));

    Facedb *Facedb::GetInstance() {
        return Facedb::instance;
    }

    Facedb::Facedb(){}

    int Facedb::AddUser(string group, string id, float *feature) {
        return db->Put(group, id, feature);
    }

    int Facedb::GetUser(string group, string id, float *feature) {
        return db->Get(group, id, feature);
    }

    int Facedb::Search(string group, float *ffeature, vector<pb::SearchReply_User> &users) {
        return db->Search(group, ffeature, users);
    }

    int Facedb::DelUser(std::string group, std::string id) {
        return db->Delete(group, id);
    }

    int Facedb::Delgroup(std::string group) {
        return db->DelGroup(group);
    }

    int Facedb::GroupList(std::vector<string> & groups) {
        return db->GroupList(groups);
    }

    int Facedb::UserList(std::string group, std::string startKey, int num, std::vector<std::string> & users) {
        return db->UserList(group,startKey,num,users);
    }

    uint64_t Facedb::GroupSize(string group) {
        return db->GroupSize(group);
    }

    Facedb::~Facedb() {
        delete Facedb::instance;
        delete db;
    }
}