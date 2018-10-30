
# centos 7

# 编译 protobuf

```
    确定centos7上已经安装了下面的软件，或者直接用yum进行更新
    autoconf
    automake
    libtool
    curl (used to download gmock)
    make
    g++
    unzip

    git clone https://github.com/google/protobuf.git
    git checkout v3.6.1
    # 如果使用的不是源码，而是release版本 (已经包含gmock和configure脚本)，可以略过这一步
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
    3、设置一下环境变量 /etc/profile
    # (动态库搜索路径) 程序加载运行期间查找动态链接库时指定除了系统默认路径之外的其他路径
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/protobuf/lib
    # (静态库搜索路径) 程序编译期间查找动态链接库时指定查找共享库的路径
    export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/protobuf/lib
    export PATH=$PATH:/usr/local/protobuf/bin
    #拷贝头文件
    cp  -r youpaht/protobuf/src/google  /usr/local/include/
    4、查看版本
    protoc --version
```


#  编译 grpc

```
    git clone https://github.com/grpc/grpc.git
    cd grpc
    git submodule update --init
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