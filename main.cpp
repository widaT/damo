#include <iostream>
#include "db/db.h"
#include "db/feature.pb.h"
using namespace db;
using namespace std;
using namespace featurepb;

int main() {
    rocksdb::Options options;
    options.create_if_missing = true;
    db::DB db(options,"/tmp/testdb");
    db.Put("key1","wida");
    string a;
    db.Get("key1",&a);
    cout << a << endl;

    db.Delete("key1");
    int ret = db.Get("key1",&a);

    cout << ret << endl;
    cout << a << endl;



    Feature feature;

    feature.add_feature(1.55);
    feature.add_feature(0.52);
    feature.add_feature(0.52);
    feature.add_feature(0.52);
    feature.add_feature(0.52);
    feature.add_feature(0.52);




    size_t length = feature.ByteSizeLong();

    cout << length << endl;
    char* buf = new char[length];
    feature.SerializeToArray(buf,length);


    cout << string(buf) << endl;

    delete buf;
    return 0;
}