#!/bin/bash
PROTOC=`which protoc`
$PROTOC --cpp_out=. search.proto
$PROTOC --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` search.proto