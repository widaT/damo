#include <iostream>
#include "db/db.h"
#include "pb/search.pb.h"
#include "pb/search.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "db/ini.h"
#include <unistd.h>
#include "db/log.h"
#include <immintrin.h>
#include "db/facedb.h"

using namespace std;
using namespace pb;
using namespace config;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

static db::Facedb *facedb =  db::Facedb::getInstance();


float avx_euclidean_distance( float *x, float *y)
{
    static const size_t single_size = 8;
    const size_t end = 512 / single_size;
    __m256 *vx = (__m256 *)x;
    __m256 *vy = (__m256 *)y;
    __m256 vsub = {0};
    __m256 vsum = {0};
    for(size_t i=0; i<end; ++i) {
        vsub = _mm256_sub_ps(vx[i], vy[i]);
        vsum = _mm256_add_ps(vsum, _mm256_mul_ps(vsub, vsub));
    }
    __attribute__((aligned(32))) float t[8] = {0};
    _mm256_store_ps(t, vsum);
    return t[0] + t[1] + t[2] + t[3] + t[4] + t[5] + t[6] + t[7];
}


float distance(float *a,float *b) {
    float distance  = 0.0;
    for (int i =0;i<512;i++) {
        float sub = *(a+i) - *(b+i);
        distance += sub * sub;
    };
    return distance;
}

class SearchServiceImpl final : public ::Facedb::Service {
    Status Search(ServerContext* context, const  SearchRequest* req, SearchReply* reply) override {

        float feature[512] = {0};
        for (int i=0;i< req->feature().size();i++){
            feature[i] = req->feature(i);
        }

        cout << req->group() << endl;
        facedb->search("",req->group(),feature);

        SearchReply_User *retUser =  reply->add_users();
        retUser->set_name("wida");
        retUser->set_distance(10);


        return Status::OK;
    }

    Status AddUser(ServerContext* context, const  UserInfo* req, NomalReply* reply) override {
        int size = req->feature().feature_size();
        float feature[512] = {0};
        for (int i=0;i< size;i++){
            feature[i] = req->feature().feature(i);
        }
        db::User user;
        user.feature = feature;
        user.id = req->id();
        facedb->addUser(user);
        reply->set_ret(true);
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