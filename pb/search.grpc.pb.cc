// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: search.proto

#include "search.pb.h"
#include "search.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace pb {

static const char* Facedb_method_names[] = {
  "/pb.Facedb/Search",
  "/pb.Facedb/GroupList",
  "/pb.Facedb/AddUser",
  "/pb.Facedb/DelUser",
};

std::unique_ptr< Facedb::Stub> Facedb::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Facedb::Stub> stub(new Facedb::Stub(channel));
  return stub;
}

Facedb::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Search_(Facedb_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GroupList_(Facedb_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_AddUser_(Facedb_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DelUser_(Facedb_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Facedb::Stub::Search(::grpc::ClientContext* context, const ::pb::SearchRequest& request, ::pb::SearchReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Search_, context, request, response);
}

void Facedb::Stub::experimental_async::Search(::grpc::ClientContext* context, const ::pb::SearchRequest* request, ::pb::SearchReply* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Search_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::pb::SearchReply>* Facedb::Stub::AsyncSearchRaw(::grpc::ClientContext* context, const ::pb::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::SearchReply>::Create(channel_.get(), cq, rpcmethod_Search_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::pb::SearchReply>* Facedb::Stub::PrepareAsyncSearchRaw(::grpc::ClientContext* context, const ::pb::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::SearchReply>::Create(channel_.get(), cq, rpcmethod_Search_, context, request, false);
}

::grpc::Status Facedb::Stub::GroupList(::grpc::ClientContext* context, const ::pb::SearchRequest& request, ::pb::GroupsReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GroupList_, context, request, response);
}

void Facedb::Stub::experimental_async::GroupList(::grpc::ClientContext* context, const ::pb::SearchRequest* request, ::pb::GroupsReply* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_GroupList_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::pb::GroupsReply>* Facedb::Stub::AsyncGroupListRaw(::grpc::ClientContext* context, const ::pb::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::GroupsReply>::Create(channel_.get(), cq, rpcmethod_GroupList_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::pb::GroupsReply>* Facedb::Stub::PrepareAsyncGroupListRaw(::grpc::ClientContext* context, const ::pb::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::GroupsReply>::Create(channel_.get(), cq, rpcmethod_GroupList_, context, request, false);
}

::grpc::Status Facedb::Stub::AddUser(::grpc::ClientContext* context, const ::pb::UserInfo& request, ::pb::NomalReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_AddUser_, context, request, response);
}

void Facedb::Stub::experimental_async::AddUser(::grpc::ClientContext* context, const ::pb::UserInfo* request, ::pb::NomalReply* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_AddUser_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::pb::NomalReply>* Facedb::Stub::AsyncAddUserRaw(::grpc::ClientContext* context, const ::pb::UserInfo& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::NomalReply>::Create(channel_.get(), cq, rpcmethod_AddUser_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::pb::NomalReply>* Facedb::Stub::PrepareAsyncAddUserRaw(::grpc::ClientContext* context, const ::pb::UserInfo& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::NomalReply>::Create(channel_.get(), cq, rpcmethod_AddUser_, context, request, false);
}

::grpc::Status Facedb::Stub::DelUser(::grpc::ClientContext* context, const ::pb::UserInfo& request, ::pb::NomalReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DelUser_, context, request, response);
}

void Facedb::Stub::experimental_async::DelUser(::grpc::ClientContext* context, const ::pb::UserInfo* request, ::pb::NomalReply* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DelUser_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::pb::NomalReply>* Facedb::Stub::AsyncDelUserRaw(::grpc::ClientContext* context, const ::pb::UserInfo& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::NomalReply>::Create(channel_.get(), cq, rpcmethod_DelUser_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::pb::NomalReply>* Facedb::Stub::PrepareAsyncDelUserRaw(::grpc::ClientContext* context, const ::pb::UserInfo& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::pb::NomalReply>::Create(channel_.get(), cq, rpcmethod_DelUser_, context, request, false);
}

Facedb::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Facedb_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Facedb::Service, ::pb::SearchRequest, ::pb::SearchReply>(
          std::mem_fn(&Facedb::Service::Search), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Facedb_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Facedb::Service, ::pb::SearchRequest, ::pb::GroupsReply>(
          std::mem_fn(&Facedb::Service::GroupList), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Facedb_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Facedb::Service, ::pb::UserInfo, ::pb::NomalReply>(
          std::mem_fn(&Facedb::Service::AddUser), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Facedb_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Facedb::Service, ::pb::UserInfo, ::pb::NomalReply>(
          std::mem_fn(&Facedb::Service::DelUser), this)));
}

Facedb::Service::~Service() {
}

::grpc::Status Facedb::Service::Search(::grpc::ServerContext* context, const ::pb::SearchRequest* request, ::pb::SearchReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Facedb::Service::GroupList(::grpc::ServerContext* context, const ::pb::SearchRequest* request, ::pb::GroupsReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Facedb::Service::AddUser(::grpc::ServerContext* context, const ::pb::UserInfo* request, ::pb::NomalReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Facedb::Service::DelUser(::grpc::ServerContext* context, const ::pb::UserInfo* request, ::pb::NomalReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace pb

