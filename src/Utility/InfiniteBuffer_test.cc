#include <algorithm>
#include <unistd.h>
#include <string>

#include "UnitTest/UnitTest.h"
#include "Base/Log.h"
#include "Base/Ptr.h"
#include "Base/Utils.h"

#include "Utility/InfiniteBuffer.h"

namespace Utility {

namespace {
uint32 kTestDataSize = 65536;
uint32 kBufferSize = 128;
}

class InfiniteBufferTest: public UnitTest {
 public:
  InfiniteBufferTest() : buffer_(kBufferSize) {}

  void setup() override {
    data_ = new byte[kTestDataSize];
    for (uint32 i = 0; i < kTestDataSize; i++) {
      data_[i] = Utils::RandomNumber(256);
    }
    receiver_ = new byte[kTestDataSize];
  }

  void teardown() override {
    delete[] data_;
    delete[] receiver_;
  }

  bool DiceToWrite() {
    return Utils::RandomNumber(2) > 0;
  }

  bool DataIsCorrect() {
    for (uint32 i = 0; i < kTestDataSize; i++) {
      if (data_[i] != receiver_[i]) {
        printf("i = %d\n", i);
        return false;
      }
    }
    return true;
  }

  void test_LoadTest() {
    uint32 readn = 0, writen = 0;
    auto indexes = Utils::RandomListFromRange(1, kTestDataSize - 1,
                                              kTestDataSize / kBufferSize - 1);
    indexes.insert(indexes.begin(), 0);
    indexes.insert(indexes.end(), kTestDataSize);
    std::sort(indexes.begin(), indexes.end());

    uint32 write_times = 0;
    while (readn < kTestDataSize || write_times < kTestDataSize / kBufferSize) {
      if (DiceToWrite() && writen < kTestDataSize) {
        uint32 writen_this_time = buffer_.Write(
            data_ + indexes[write_times],
            indexes[write_times + 1] - indexes[write_times]);
        writen += writen_this_time;
        write_times++;
        //printf("writen = %d\n", writen);
      } else {
        uint32 to_read = Utils::RandomNumber(kBufferSize * 2);
        uint32 read_this_time = buffer_.Read(receiver_ + readn, to_read);
        readn += read_this_time;
        //printf("readn = %d\n", readn);
      }
    }

    AssertTrue(DataIsCorrect());
    AssertTrue(readn == kTestDataSize);
    AssertTrue(writen == kTestDataSize);
  }

 protected:
  InfiniteBuffer buffer_;

  byte* data_ = nullptr;
  byte* receiver_ = nullptr;
};

}  // namespace net_stack

int main() {
  Utility::InfiniteBufferTest test;
  test.setup();
  test.test_LoadTest();
  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}
