# etcdv3

```sh
protoc -I. --grpc_out=.  --plugin=protoc-gen-grpc=../grpc/bins/opt/grpc_cpp_plugin  test.proto
protoc -I . --cpp_out=.  test.proto
```
