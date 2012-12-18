#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <stdlib.h>
#include <sstream>
#include "ProtobufTester.h"

using namespace google::protobuf::io;

namespace sertest {
    ProtobufTestData::ProtobufTestData(uint32_t num) :
            num_(num),
            objs_(NULL) {
        objs_ = new PbObject[num_];
    }

    ProtobufTestData::~ProtobufTestData() {
        if (objs_)
            delete[] objs_;
        num_ = 0;
    }

    ProtobufTester::ProtobufTester() {
    }

    ProtobufTester::~ProtobufTester() {
    }

    const TestData& ProtobufTester::GenerateData(uint32_t num) {
        ProtobufTestData* d = new ProtobufTestData(num);
        for (uint32_t i = 0; i < num; ++i) {
            d->objs_[i].set_key(gen_random_string(8));
            d->objs_[i].set_value(rand());
        }
        return *d;
    }

    bool ProtobufTester::SerializeData(const TestData& t, void* data,
            uint32_t& ser_size) {
        std::stringstream ss;
        OstreamOutputStream* os = new OstreamOutputStream(&ss);
        CodedOutputStream* cs = new CodedOutputStream(os);
        const ProtobufTestData& pt = dynamic_cast<const ProtobufTestData&>(t);
        for (uint32_t i = 0; i < pt.num_; ++i) {
            pt.objs_[i].serialize(cs);
        }
        delete cs;
        delete os;
        ser_size = ss.str().size();
        memcpy(data, ss.str().data(), ser_size);
        return true;
    }

    TestData& ProtobufTester::DeserializeData(void* data, uint32_t ser_size,
            uint32_t num) {
        typedef boost::iostreams::stream<boost::iostreams::array_source> array_stream;
        array_stream ss((char*)data, ser_size);
        IstreamInputStream* ii = new IstreamInputStream(&ss);
        CodedInputStream* ci = new CodedInputStream(ii);
        ci->SetTotalBytesLimit(1073741824, -1);
        ProtobufTestData* d = new ProtobufTestData(num);
        for (uint32_t i = 0; i < num; ++i) {
            d->objs_[i].deserialize(ci);
        }
        return *d;
    }

    bool ProtobufTester::Compare(const TestData& t1, const TestData& t2) {
        const ProtobufTestData& pt1 = dynamic_cast<const ProtobufTestData&>(t1);
        const ProtobufTestData& pt2 = dynamic_cast<const ProtobufTestData&>(t2);
        if (pt1.num_ != pt2.num_)
            return false;

        bool ret = true;
        for (uint32_t i = 0; i < pt1.num_; ++i) {
            ret &= (pt1.objs_[i].key().compare(pt2.objs_[i].key()) == 0);
            ret &= (pt1.objs_[i].value() == pt2.objs_[i].value());
            if (!ret)
                break;
        }
        return ret;
    }

    const std::string& ProtobufTester::gen_random_string(uint32_t length) {
        std::string* result = new std::string();
        result->resize(length);
        for (uint32_t i = 0; i < length; i++)
            (*result)[i] = 97 + rand() % 26;
        return *result;
    }
}
