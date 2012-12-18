#ifndef SER_MSGPACKTESTER_H
#define SER_MSGPACKTESTER_H

#include <msgpack.hpp>
#include <stdint.h>
#include <string>
#include "test.pb.h"
#include "Tester.h"

namespace sertest {
    class MpObject {
      public:
        const std::string& key() const {
            return m_key;
        }
        const uint32_t value() const {
            return m_val;
        }
        void set_key(const std::string& k) {
            m_key = k;
        }
        void set_value(const uint32_t v) {
            m_val = v;
        }
      private:
        std::string m_key;
        uint32_t m_val;
      public:
        MSGPACK_DEFINE(m_key, m_val);
    };

    class MsgpackTestData : public TestData {
        friend class MsgpackTester;

        MsgpackTestData(uint32_t num);
        ~MsgpackTestData();

      private:
        uint32_t num_;
        MpObject* objs_;
    };

    class MsgpackTester : public Tester {
      public:
        MsgpackTester();
        ~MsgpackTester();
        const TestData& GenerateData(uint32_t num);
        bool SerializeData(const TestData& t, void* data, uint32_t& size);
        TestData& DeserializeData(void* data, uint32_t size, uint32_t num);
        bool Compare(const TestData& t1, const TestData& t2);
      private:
        const std::string& gen_random_string(uint32_t length);
    };
}

#endif  // SER_PROTOBUFTESTER_H
