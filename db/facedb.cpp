//
// Created by wida on 18-10-19.
//

#include "facedb.h"
#include "../pb/search.pb.h"
#include <iostream>
#include <sstream>
using namespace std;
namespace db {


    void pack(float *ffeature,string & sfeature){
        ostringstream f;
        f.write((char*)ffeature, 200*sizeof(float));      //fwrite以char *的方式进行写出，做一个转化
        sfeature = f.str();
       // f.clear();
    };

    void unpack(float *ffeature,string & sfeature) {
        istringstream r = istringstream(sfeature);
        r.read((char *)ffeature,200*sizeof(float));
        //r.clear();
    }



    Facedb *Facedb::instance = new Facedb();
    DB *db = new DB("/tmp/testdb");
    Facedb *Facedb::getInstance() {
        return Facedb::instance;
    }

    Facedb::Facedb() {}
    int Facedb::addUser(User user) {
        string sfeature;
        pack(user.feature,sfeature);
        return db->Put(user.id, sfeature);
    }

    int Facedb::search(string group,string id, float * ffeature) {
        string sfeature;
        if (db->Get(id,&sfeature) == 0) {
            float feautre[512] = {0};
            unpack(feautre,sfeature);
            for(int i = 0; i < 512; i++)
                cout << feautre[i] <<endl;
        }

        return 0;
    }

    Facedb::~Facedb() {
        delete Facedb::instance;
        delete db;
    };
}