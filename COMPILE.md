
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
    git clone https://github.com/grpc/grpc.git
    cd grpc
    git submodule update --init


    编译 protobuf
    cd third_party/protobuf
     ./autogen.sh
        # 指定安装路径
        ./configure --prefix=/usr/local/protobuf
        #编译
        make
        # 测试
        make check
        make install
        # refresh shared library cache.
        ldconfig
        cp  -r youpaht/protobuf/src/google  /usr/local/include/



    #编译grpc
    cd ../../
    make
    make install
```


# 编译rocksdb



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