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

    int Facedb::AddUser(const string &group, const string &id, float *feature) {
        return db->Put(group, id, feature);
    }

    int Facedb::GetUser(const string &group, const string &id, float *feature) {
        return db->Get(group, id, feature);
    }

    int Facedb::Search(const string &group, float *ffeature, vector<pb::SearchReply_User> &users) {
        return db->Search(group, ffeature, users);
    }

    int Facedb::DelUser(const string &group, const std::string &id) {
        return db->Delete(group, id);
    }

    int Facedb::Delgroup(const string &group) {
        return db->DelGroup(group);
    }

    int Facedb::GroupList(vector<string> & groups) {
        return db->GroupList(groups);
    }

    int Facedb::UserList(const string &group, const string &startKey, int num, vector<string> & users) {
        return db->UserList(group,startKey,num,users);
    }

    uint64_t Facedb::GroupSize(const string &group) {
        return db->GroupSize(group);
    }

    int Facedb::Info(pb::InfoReply & infoReply){
        return db->Info(infoReply);
    }

    Facedb::~Facedb() {
        delete Facedb::instance;
        delete db;
    }
}