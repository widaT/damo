all:zz

zz:zz.cc auth.pb.cc  etcdserver.pb.cc  kv.pb.cc  rpc.grpc.pb.cc  rpc.pb.cc
	g++ $^ -o $@ -std=c++11 -I/opt/grpc/include -I/opt/protobuf/include -I./ -L/opt/protobuf/lib -lprotobuf -L/opt/grpc/lib -lgrpc -lgrpc++
