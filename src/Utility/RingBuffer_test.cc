#include <unistd.h>
#include <string>

#include "UnitTest/UnitTest.h"
#include "Base/Log.h"
#include "Base/Ptr.h"
#include "Base/Utils.h"

#include "Utility/RingBuffer.h"

namespace Utility {

namespace {
uint32 kTestDataSize = 65536;
uint32 kBufferSize = 128;
}

class RingBufferTest: public UnitTest {
 public:
  RingBufferTest() : ring_buffer_(kBufferSize) {}

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
        return false;
      }
    }
    return true;
  }

  void test_LoadTest() {
    uint32 readn = 0, writen = 0;
    while (readn < kTestDataSize || writen < kTestDataSize) {
      if (writen < kTestDataSize && DiceToWrite() && !ring_buffer_.full()) {
        // Do write.
        uint32 to_write = Utils::RandomNumber(kBufferSize);
        to_write = Utils::Min(kTestDataSize - writen, to_write);
        uint32 writen_this_time = ring_buffer_.Write(data_ + writen, to_write);
        // printf("writen_this_time = %u\n", writen_this_time);
        writen += writen_this_time;
      } else if (!ring_buffer_.empty()) {
        // Do read.
        uint32 to_read = Utils::RandomNumber(kBufferSize);
        uint32 readn_this_time = ring_buffer_.Read(receiver_ + readn, to_read);
        // printf("readn_this_time = %u\n", readn_this_time);
        readn += readn_this_time;
        // printf("readn = %d\n", readn);
      }
    }

    AssertTrue(DataIsCorrect());
    AssertTrue(readn == kTestDataSize);
    AssertTrue(writen == kTestDataSize);
  }

 protected:
  RingBuffer ring_buffer_;

  byte* data_ = nullptr;
  byte* receiver_ = nullptr;
};

}  // namespace net_stack

int main() {
  Utility::RingBufferTest test;
  test.setup();
  test.test_LoadTest();
  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}
