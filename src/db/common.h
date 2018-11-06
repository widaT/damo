//
// Created by wida on 18-10-22.
//

#ifndef DAMO_COMMON_H
#define DAMO_COMMON_H
#include <immintrin.h>
#include <sstream>
#include <iostream>
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

#endif //DAMO_COMMON_H
