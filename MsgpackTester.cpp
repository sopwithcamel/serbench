#include <stdlib.h>
#include <sstream>
#include "MsgpackTester.h"

using namespace google::protobuf::io;

namespace sertest {
    MsgpackTestData::MsgpackTestData(uint32_t num) :
            num_(num),
            objs_(NULL) {
        objs_ = new MpObject[num_];
    }

    MsgpackTestData::~MsgpackTestData() {
        if (objs_)
            delete[] objs_;
        num_ = 0;
    }

    MsgpackTester::MsgpackTester() {
    }

    MsgpackTester::~MsgpackTester() {
    }

    const TestData& MsgpackTester::GenerateData(uint32_t num) {
        MsgpackTestData* d = new MsgpackTestData(num);
        for (uint32_t i = 0; i < num; ++i) {
            d->objs_[i].set_key(gen_random_string(8));
            d->objs_[i].set_value(rand());
        }
        return *d;
    }

    bool MsgpackTester::SerializeData(const TestData& t, void* data,
            uint32_t& size) {
        msgpack::sbuffer buffer;
        msgpack::packer<msgpack::sbuffer> pk(&buffer);
        const MsgpackTestData& pt = dynamic_cast<const MsgpackTestData&>(t);
        for (uint32_t i = 0; i < pt.num_; ++i) {
            pk.pack(pt.objs_[i]);
        }
        size = buffer.size();
        memcpy(data, buffer.data(), size);
        return true;
    }

    TestData& MsgpackTester::DeserializeData(void* data, uint32_t ser_size,
            uint32_t num) {
        msgpack::unpacker pac;
        // feeds the buffer.
        pac.reserve_buffer(ser_size);
        memcpy(pac.buffer(), data, ser_size);
        pac.buffer_consumed(ser_size);

        msgpack::unpacked result;
        MsgpackTestData* d = new MsgpackTestData(num);
        uint32_t i = 0;
        while(pac.next(&result)) {
            result.get().convert(&(d->objs_[i]));
            ++i;
        }
        return *d;
    }

    bool MsgpackTester::Compare(const TestData& t1, const TestData& t2) {
        const MsgpackTestData& pt1 = dynamic_cast<const MsgpackTestData&>(t1);
        const MsgpackTestData& pt2 = dynamic_cast<const MsgpackTestData&>(t2);
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

    const std::string& MsgpackTester::gen_random_string(uint32_t length) {
        std::string* result = new std::string();
        result->resize(length);
        for (uint32_t i = 0; i < length; i++)
            (*result)[i] = 97 + rand() % 26;
        return *result;
    }
}
