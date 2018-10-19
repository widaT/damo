#include <iostream>
#include "db/db.h"
#include "db/feature.pb.h"
#include "pb/search.pb.h"
#include "pb/search.grpc.pb.h"
#include <grpcpp/grpcpp.h>


using namespace db;
using namespace std;
using namespace featurepb;
using namespace pb;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;



class SearchServiceImpl final : public Searcher::Service {
    Status Search(ServerContext* context, const  SearchRequest* req,
    SearchReply* reply) override {
        SearchReply_User *user  =reply->add_users();
        user->set_name("wida",4);
        user->set_distance(5.6);

        reply->add_users()->set_name("wwwxxxx",7);
        reply->add_users()->set_distance(8.885);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    SearchServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.



    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
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

    RunServer();

    return 0;

}