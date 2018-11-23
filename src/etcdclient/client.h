//
// Created by wida on 18-11-23.
//

#ifndef DAMO_CLIENT_H
#define DAMO_CLIENT_H
#include <memory>
#include <string>

#include <grpc/grpc.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/client_context.h>

#include <grpcpp/grpcpp.h>

#include "rpc.grpc.pb.h"
#include "rpc.pb.h"



class EtcdClient {

public:
    EtcdClient(std::string url);
    bool put(const std::string &, const std::string &);
private:
    std::unique_ptr<etcdserverpb::KV::Stub> stub;
};
#endif //DAMO_CLIENT_H
