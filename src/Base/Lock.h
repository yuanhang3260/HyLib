#ifndef BASE_LOCK_
#define BASE_LOCK_

#include <atomic>

namespace base {

class SpinLock {
 public:
  ~SpinLock() {
    unlock();
  }

  void lock() {
    while (locked.test_and_set(std::memory_order_acquire)) {}
  }

  void unlock() {
    locked.clear(std::memory_order_release);
  }

 private:
  std::atomic_flag locked = ATOMIC_FLAG_INIT;
};

}  // namespace base

#endif  /* BASE_LOCK_ */
