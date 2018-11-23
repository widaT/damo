#include "client.h"

using namespace etcdserverpb;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter;


EtcdClient::EtcdClient(std::string url) {
    grpc_init();
    auto channel = grpc::CreateChannel(url, grpc::InsecureChannelCredentials());
    stub = KV::NewStub(channel);
}

bool EtcdClient::put(const std::string & key, const std::string & val) {
    ClientContext context;
    PutRequest request;
    PutResponse response;
    request.set_key(key);
    request.set_value(val);
    auto ret = stub->Put(&context, request, &response);
    return ret.ok();
}