#include <iostream>
#include "db/db.h"
#include "pb/search.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "db/ini.h"
#include <unistd.h>
#include "db/log.h"
#include "db/facedb.h"

using namespace std;
using namespace pb;
using namespace config;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

static db::Facedb *facedb =  db::Facedb::getInstance();

class SearchServiceImpl final : public ::Facedb::Service {
    Status Search(ServerContext* context, const  SearchRequest* req, SearchReply* reply) override {
        float feature[512] = {0};
        for (int i=0;i< req->feature().size();i++){
            feature[i] = req->feature(i);
        }
        vector<SearchReply_User> user;
        facedb->search(req->group(),feature,user);
        if (!user.empty()) {
            for (auto iter = user.begin();iter != user.end();iter++){
                SearchReply_User *retUser =  reply->add_users();
                retUser->set_name(iter->name());
                retUser->set_distance(iter->distance());
            }
        }else {
            reply->clear_users();
        }

        return Status::OK;
    }

    Status AddUser(ServerContext* context, const  UserInfo* req, NomalReply* reply) override {
        int size = req->feature().feature_size();
        float feature[512] = {0};
        for (int i=0;i< size;i++){
            feature[i] = req->feature().feature(i);
        }

        int ret = facedb->addUser(req->group(),req->id(),feature);
        reply->set_ret(0 == ret ? true : false);
        return Status::OK;
    }
};

string cwd() {
    char pwd[255];
    if(!getcwd(pwd,255)){
        perror("getcwd");
        return "";
    }
    return string(pwd);
}


int  RunServer() {
    INI *conf = INI::getInstance();
    if (!conf->open("/home/wida/cppworkspace/damo/etc/conf.conf")){
        cout << "dd" <<endl;
        return  -1;
    }
    string port  = conf->read("base","port");
    std::string server_address("0.0.0.0:"+port);
    SearchServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    app_log_init("/home/wida/cppworkspace/damo/log/",_APP_TRACE,"log.",0);
    //APP_WARN_LOG("fsdfdsf %s","aaa");
    RunServer();
    return 0;
}