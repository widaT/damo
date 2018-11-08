#   damo 人脸特征值存储，比对引擎

##  features

- 采用grpc 支持多语言客户端

- 使用rocksdb做存储 百万人脸特征库 1：n 性能为1-3秒

- 采用avx指令指令集优化比对

- 支持物理机和docker 运行

- 采用cmake构建至少cmake3.8 c++11

##  docker

```
docker build -t damo:v1 .
docker run -id -p 8010:50051 -v /home/wida/dockerdata/damo:/data/app/data:rw  --name damov1 damo:v1
```