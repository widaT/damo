#include <iostream>
#include "db/db.h"
#include "pb/search.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "db/ini.h"
#include "db/facedb.h"
#include "db/common.h"

using namespace std;
using namespace pb;
using namespace config;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

static db::Facedb *facedb = db::Facedb::getInstance();

class SearchServiceImpl final : public ::Facedb::Service {
    Status Search(ServerContext *context, const SearchRequest *req, SearchReply *reply) override {
        float feature[FEATURE_SIZE] = {0};
        for (int i = 0; i < req->feature().size(); i++) {
            feature[i] = req->feature(i);
        }
        vector<SearchReply_User> user;
        facedb->search(req->group(), feature, user);
        if (!user.empty()) {
            for (auto iter = user.begin(); iter != user.end(); iter++) {
                auto *retUser = reply->add_users();
                retUser->set_name(iter->name());
                retUser->set_distance(iter->distance());
            }
        } else {
            reply->clear_users();
        }
        return Status::OK;
    }

    Status AddUser(ServerContext *context, const UserInfo *req, NomalReply *reply) override {
        int size = req->feature_size();
        float feature[FEATURE_SIZE] = {0};
        for (int i = 0; i < size; i++) {
            feature[i] = req->feature(i);
        }
        int ret = facedb->addUser(req->group(), req->id(), feature);
        reply->set_ret(0 == ret);
        return Status::OK;
    }

    Status GetUser(ServerContext *context, const UserInfo *req, UserInfo *reply) override {
        float feature[FEATURE_SIZE] = {0};
        int ret = facedb->getUser(req->group(), req->id(), feature);
        if (ret == 0) {
            reply->set_id(req->id());
            reply->set_group(req->group());
            for (auto i : feature) {
                reply->add_feature(i);
            }
        }
        return Status::OK;
    }

    Status DelUser(ServerContext *context, const UserInfo *req, NomalReply *reply) override {
        int ret = facedb->delUser(req->group(), req->id());
        reply->set_ret(ret == 0);
        return Status::OK;
    }

    Status GroupList(ServerContext *context, const Null *req, StringsReply *reply) override {
        vector<string> groups;
        int ret = facedb->groupList(groups);
        if (ret == -1) {
            reply->add_values();
        }else {
            for(auto i:groups) {
                reply->add_values(i);
            }
        }
        return Status::OK;
    }


    Status UserList(ServerContext *context, const UserListReq *req, StringsReply *reply) override {
        vector<string> users;
        int ret = facedb->userList(req->group(),req->skey(),req->num(),users);
        if (ret == -1) {
            reply->add_values();
        }else {
            for(auto i:users) {
                reply->add_values(i);
            }
        }
        return Status::OK;
    }

    Status DelGroup(ServerContext *context, const Group *req, NomalReply *reply) override {
        reply->set_ret(facedb->delgroup(req->group()) == 0);
        return Status::OK;
    }
};



int RunServer() {
    INI *conf = INI::getInstance();
    string port = conf->read("base", "port");
    std::string server_address("0.0.0.0:" + port);
    SearchServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
   // APP_WARN_LOG("fsdfdsf %s","aaa");
    RunServer();
    return 0;
}