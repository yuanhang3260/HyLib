#include <chrono>
#include <thread>

#include "Base/BaseTypes.h"
#include "Base/Lock.h"
#include "UnitTest/UnitTest.h"

namespace base {

class LockTest: public UnitTest {
 public:
  void Test_SpinLock() {
    SpinLock lock;
    int value = 0;
    auto incrementor = [&] {
      for (uint32 i = 0; i < 10; i++) {
        lock.lock();
        int prev_value = value;
        // Create a race condition - Sleep for 0.1s and check the value again.
        // If value is not changed by another thread (no race), increment it.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (prev_value == value) {
          value++;
        }
        lock.unlock();
      }
    };

    std::thread thread1(incrementor);
    std::thread thread2(incrementor);

    thread1.join();
    thread2.join();

    AssertEqual(20, value);
  }
};

}  // namespace base

int main() {
  base::LockTest test;
  test.Test_SpinLock();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}