#ifndef SER_PROTOBUFTESTER_H
#define SER_PROTOBUFTESTER_H

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <stdint.h>
#include <string>
#include "test.pb.h"
#include "Tester.h"

namespace sertest {
    class PbObject {
      public:
        const std::string& key() const {
            return pb.key();
        }
        const uint32_t value() const {
            return pb.value();
        }
        void set_key(const std::string& k) {
            pb.set_key(k);
        }
        void set_value(const uint32_t v) {
            pb.set_value(v);
        }
        void serialize(google::protobuf::io::CodedOutputStream* cs) const {
            cs->WriteVarint32(pb.ByteSize());
            pb.SerializeToCodedStream(cs);
        }
        void deserialize(google::protobuf::io::CodedInputStream* ci) {
            uint32_t bytes;
            ci->ReadVarint32(&bytes);
            google::protobuf::io::CodedInputStream::Limit msgLimit =
                    ci->PushLimit(bytes);
            pb.ParseFromCodedStream(ci);
            ci->PopLimit(msgLimit);
        }
      private:
        test::obj pb;
    };

    class ProtobufTestData : public TestData {
        friend class ProtobufTester;

        ProtobufTestData(uint32_t num);
        ~ProtobufTestData();

      private:
        uint32_t num_;
        PbObject* objs_;
    };

    class ProtobufTester : public Tester {
      public:
        ProtobufTester();
        ~ProtobufTester();
        const TestData& GenerateData(uint32_t num);
        bool SerializeData(const TestData& t, void* data, uint32_t& size);
        TestData& DeserializeData(void* data, uint32_t size, uint32_t num);
        bool Compare(const TestData& t1, const TestData& t2);
      private:
        const std::string& gen_random_string(uint32_t length);
    };
}

#endif  // SER_PROTOBUFTESTER_H
