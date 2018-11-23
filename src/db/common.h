//
// Created by wida on 18-10-22.
//

#pragma once
#ifndef DAMO_COMMON_H
#define DAMO_COMMON_H
#include <immintrin.h>
#include <sstream>
#include <iostream>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sys/time.h>

#define FEATURE_SIZE 512  //特征值大小

float distance(float *, float *);
void pack(float *, std::string &);
void unpack(float *, std::string &);
std::string cwd();
inline float avx_euclidean_distance( float *x, float *y)
{
    __m256 vsub,vsum={0},v1,v2;
    for(size_t i=0; i < FEATURE_SIZE; i=i+8) {
        v1  = _mm256_loadu_ps(x+i);
        v2  = _mm256_loadu_ps(y+i);
        vsub = _mm256_sub_ps(v1, v2);
        vsum = _mm256_add_ps(vsum, _mm256_mul_ps(vsub, vsub));
    }
    __attribute__((aligned(32))) float t[8] = {0};
    _mm256_store_ps(t, vsum);
    return t[0] + t[1] + t[2] + t[3] + t[4] + t[5] + t[6] + t[7];
}

inline  std::string get_local_ip(){
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            //去掉还回地址
            if (strstr(addressBuffer,"127") == NULL) {
                ifAddrStruct=ifAddrStruct->ifa_next;
                return addressBuffer;
            }
        } /*else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer);
        }*/
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
}

inline  std::string get_local_time() {
    struct timeval tv;
    char buf[21];
    gettimeofday(&tv,NULL);
    strftime(buf,sizeof(buf) - 1,"%Y-%m-%d %H:%M:%S",localtime(&tv.tv_sec));
    return buf;
}
#endif //DAMO_COMMON_H
