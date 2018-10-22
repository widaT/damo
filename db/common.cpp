//
// Created by wida on 18-10-22.
//

#include "common.h"

using namespace std;

float distance(float *a, float *b) {
    float distance = 0.0;
    for (int i = 0; i < FEATURE_SIZE; i++) {
        float sub = *(a + i) - *(b + i);
        distance += sub * sub;
    };
    return distance;
}

void pack(float *ffeature, string &sfeature) {
    ostringstream f;
    f.write((char *) ffeature, FEATURE_SIZE * sizeof(float)); //fwrite以char *的方式进行写出，做一个转化
    sfeature = f.str();
};

void unpack(float *ffeature, string &sfeature) {
    istringstream r = istringstream(sfeature);
    r.read((char *) ffeature, FEATURE_SIZE * sizeof(float));
}