#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "ProtobufTester.h"
#include "MsgpackTester.h"

float time_diff(struct timeval& t1, struct timeval& t2) {
    uint32_t a = (t1.tv_sec * 1000000 + t1.tv_usec);
    uint32_t b = (t2.tv_sec * 1000000 + t2.tv_usec);
    return ((float)(b - a)) / 1000000;
}

int main(int argc, char* argv[]) {
    uint32_t num = 10000000;
    uint32_t bufsize = num * 100;
    uint32_t opt;

    if (argc < 2)
        opt = 0;
    else
        opt = atoi(argv[1]);

    void* ser_data = malloc(bufsize);
    std::string ser_string;
    srand(213123);
    sertest::Tester* pt;
    switch(opt) {
        case 0:
            fprintf(stderr, "Protobuf\n");
            pt = dynamic_cast<sertest::ProtobufTester*>(new sertest::ProtobufTester());
            break;
        case 1:
            fprintf(stderr, "Msgpack\n");
            pt = dynamic_cast<sertest::MsgpackTester*>(new sertest::MsgpackTester());
            break;
    }

    const sertest::TestData& t = pt->GenerateData(num);
    struct timeval tv1, tv2, tv3; 
    uint32_t serialized_size;

    gettimeofday(&tv1, 0);
    assert(pt->SerializeData(t, ser_data, serialized_size));
    gettimeofday(&tv2, 0);
    const sertest::TestData& t2 = pt->DeserializeData(ser_data, serialized_size, num);
    gettimeofday(&tv3, 0);
    if (!pt->Compare(t, t2))
        fprintf(stderr, "Compare failed\n");

    fprintf(stderr, "Serialized size: %u\n", serialized_size);
    fprintf(stderr, "Serializing time: %f\n", time_diff(tv1, tv2));
    fprintf(stderr, "Deserializing time: %f\n", time_diff(tv2, tv3));
    delete pt;
}
