#include <iostream>
#include "db/db.h"
#include "db/feature.pb.h"
#include "pb/search.pb.h"
#include "pb/search.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include "db/ini.h"
#include <unistd.h>
#include "db/log.h"
#include <immintrin.h>

using namespace db;
using namespace std;
using namespace featurepb;
using namespace pb;
using namespace config;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

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

class SearchServiceImpl final : public Facedb::Service {
    Status Search(ServerContext* context, const  SearchRequest* req, SearchReply* reply) override {
        SearchReply_User *user  =reply->add_users();
        user->set_name("wida",4);
        user->set_distance(5.6);
        reply->add_users()->set_name("wwwxxxx",7);
        reply->add_users()->set_distance(8.885);
        return Status::OK;
    }

    Status AddUser(ServerContext* context, const  UserInfo* req, NomalReply* reply) override {
        int size = req->feature_size();
        Feature feature;
        for (int i=0;i< size;i++){
            feature.add_feature(req->feature(i));
        }
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
    //cout << cwd() << endl;
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
    /*rocksdb::Options options;
    options.create_if_missing = true;
    db::DB db(options,"/tmp/testdb");
    db.Put("key1","wida");
    string a;
    db.Get("key1",&a);
    cout << a << endl;

    db.Delete("key1");
    int ret = db.Get("key1",&a);

    cout << ret << endl;
    cout << a << endl;



    Feature feature;

    feature.add_feature(1.55);
    feature.add_feature(0.52);
    feature.add_feature(0.52);
    feature.add_feature(0.52);
    feature.add_feature(0.52);
    feature.add_feature(0.52);




    size_t length = feature.ByteSizeLong();

    cout << length << endl;
    char* buf = new char[length];
    feature.SerializeToArray(buf,length);


    cout << string(buf) << endl;

    delete buf;
    return 0;*/

    app_log_init("/home/wida/cppworkspace/damo/log/",_APP_TRACE,"log.",0);


    APP_WARN_LOG("fsdfdsf %s","aaa");

    RunServer();

    return 0;

}