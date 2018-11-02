#include <iostream>
#include "db/db.h"
#include "pb/search.grpc.pb.h"
#include <grpc++/grpc++.h>
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

static db::Facedb *facedb = db::Facedb::GetInstance();

class SearchServiceImpl final : public ::Facedb::Service {
    Status Search(ServerContext *context, const SearchRequest *req, SearchReply *reply) override {
        float feature[FEATURE_SIZE] = {0};
        for (int i = 0; i < req->feature().size(); i++) {
            feature[i] = req->feature(i);
        }
        vector<SearchReply_User> user;
        facedb->Search(req->group(), feature, user);
        for (auto &iter : user) {
            SearchReply_User *retUser;
            retUser = reply->add_users();
            retUser->set_name(iter.name());
            retUser->set_distance(iter.distance());
        }
        return Status::OK;
    }

    Status AddUser(ServerContext *context, const UserInfo *req, NomalReply *reply) override {
        int size = req->feature_size();
        float feature[FEATURE_SIZE] = {0};
        for (int i = 0; i < size; i++) {
            feature[i] = req->feature(i);
        }
        int ret = facedb->AddUser(req->group(), req->id(), feature);
        reply->set_ret(0 == ret);
        return Status::OK;
    }

    Status GetUser(ServerContext *context, const UserInfo *req, UserInfo *reply) override {
        float feature[FEATURE_SIZE] = {0};
        int ret = facedb->GetUser(req->group(), req->id(), feature);
        if (ret == 0) {
            reply->set_id(req->id());
            reply->set_group(req->group());
            for (auto &i:feature) {
                reply->add_feature(i);
            }
        }
        return Status::OK;
    }

    Status DelUser(ServerContext *context, const UserInfo *req, NomalReply *reply) override {
        int ret = facedb->DelUser(req->group(), req->id());
        reply->set_ret(ret == 0);
        return Status::OK;
    }

    Status GroupList(ServerContext *context, const Null *req, StringsReply *reply) override {
        vector<string> groups;
        facedb->GroupList(groups);
        for (auto &i:groups) {
            reply->add_values(i);
        }
        return Status::OK;
    }


    Status UserList(ServerContext *context, const UserListReq *req, StringsReply *reply) override {
        vector<string> users;
        facedb->UserList(req->group(), req->skey(), req->num(), users);
        for (auto &i:users) {
            reply->add_values(i);
        }
        return Status::OK;
    }

    Status DelGroup(ServerContext *context, const Group *req, NomalReply *reply) override {
        reply->set_ret(facedb->Delgroup(req->group()) == 0);
        return Status::OK;
    }

    Status GroupSize(ServerContext *context, const Group *req, SizeReply *reply) override {
        reply->set_size(facedb->GroupSize(req->group()));
        return Status::OK;
    }

    Status Info(ServerContext *context, const Null *request, InfoReply *reply) override {
      //  InfoReply reply;
        facedb->Info(*reply);
        return Status::OK;
    }
};


int RunServer() {
    INI *conf = INI::GetInstance();
    string port = conf->Read("base", "port");
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