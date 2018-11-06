FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y build-essential autoconf libtool curl git

RUN git clone -b $(curl -L http://grpc.io/release) https://github.com/grpc/grpc \
    && cd grpc \
    && git submodule update --init \
    && make \
    && make install \
    && cd third_party/protobuf \
    && make \
    && make install \
    && rm -rf /grpc

RUN apt-get install -y libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev \
    && git clone https://github.com/facebook/rocksdb.git \
    && cd rocksdb \
    && make shared_lib \
    && make install-shared \
    && rm -rf /rocksdb


RUN apt-get install -y wget \
    && wget https://cmake.org/files/v3.11/cmake-3.11.4.tar.gz \
    && tar xzvf cmake-3.11.4.tar.gz \
    && cd cmake-3.11.4 \
    && ./configure \
    && make \ 
    && make install \
    && rm -rf /cmake-3.11.4.tar.gz /cmake-3.11.4 \
    && apt-get remove -y wget


RUN mkdir -p /build
COPY CMakeLists.txt /build
COPY src /build/src

RUN cd /build \
    && cmake  -DCMAKE_BUILD_TYPE=Realse . \
    && cd src/pb && sh ./gen.sh && cd ../../ \
	&& make 

RUN mkdir -p /data/app/etc \
    && mkdir -p /data/app/log \
    && cp /build/src/damo /data/app 

COPY etc/conf_release.conf /data/app/etc/conf.conf

ENV LD_LIBRARY_PATH usr/lib:/usr/local/lib
ENV PATH /data/app:$PATH

EXPOSE 8010
WORKDIR /data/app
ENTRYPOINT ["damo"]