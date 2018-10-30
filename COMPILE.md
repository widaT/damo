
# centos 7

# 安装依赖

```
    确定centos7上已经安装了下面的软件，或者直接用yum进行更新
    autoconf
    automake
    libtool
    curl (used to download gmock)
    make
    g++
    unzip
```


#  编译 grpc

```
    #下载grpc
    git clone https://github.com/grpc/grpc.git
    cd grpc
    #下载子模块
    git submodule update --init


    #编译 protobuf
    cd third_party/protobuf
    ./autogen.sh
    # 指定安装路径
    ./configure --prefix=/usr/local/protobuf
    #编译
    make
    # 测试
    make check
    sudo make install
    # refresh shared library cache.
    ldconfig
    #sudo cp -r src/google  /usr/local/include/

    #编译grpc
    cd ../../
    make
    sudo make install
```

# 编译rocksdb
```
    #step1
    先安装 https://github.com/facebook/rocksdb/blob/master/INSTALL.md 准备编译环境

    #step2
    git clone https://github.com/facebook/rocksdb.git
    cd rocksdb
    git checkout 5.17.fb
    sudo make shared_lib


    #step3
    #进行接下来三步操作解决编译链接目录问题
    sudo cp librocksdb.so /usr/local/lib
    sudo ln -s /usr/local/lib/librocksdb.so /usr/local/lib/librocksdb.so.5.xxx
    sudo cp -r ./include/* /usr/local/include/
    #修改 vim /etc/profile 添加
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```

# 安装cmake 至少3.8

```
  wget https://cmake.org/files/v3.11/cmake-3.11.4.tar.gz
  tar xzvf cmake-3.11.4.tar.gz
  cd cmake-3.11.4
  ./configure
  make
  sudo make install
 ```

# 编译damo

```
  cd pb
  ./gen.sh
  cmake
  make
```