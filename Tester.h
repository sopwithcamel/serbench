#ifndef SER_TESTER_H
#define SER_TESTER_H

namespace sertest {
    class TestData {
      public:
        TestData() {}
        virtual ~TestData() {}
    };

    class Tester {
      public:
        Tester() {}
        virtual ~Tester() {}
        virtual const TestData& GenerateData(uint32_t num) = 0;
        // serialize test data from t to string s
        virtual bool SerializeData(const TestData& t, void* data,
                uint32_t& size) = 0;
        // deserialize from string s
        virtual const TestData& DeserializeData(void* data, uint32_t size,
                uint32_t num) = 0;
        virtual bool Compare(const TestData& t1, const TestData& t2) = 0;
    };
}

#endif  // SER_TESTER_H
