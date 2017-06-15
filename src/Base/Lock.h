#ifndef BASE_LOCK_
#define BASE_LOCK_

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Base/MacroUtils.h"

namespace base {

class SpinLock {
 public:
  SpinLock() = default;

  void lock() {
    while (locked.test_and_set(std::memory_order_acquire)) {}
  }

  void unlock() {
    locked.clear(std::memory_order_release);
  }

 private:
  std::atomic_flag locked = ATOMIC_FLAG_INIT;

  FORBID_COPY_AND_ASSIGN(SpinLock);
};

// This is a writer-favored read-write lock. Readers can be starved.
class RWLock {
 public:
  RWLock() = default;
  ~RWLock();

  void ReadLock();
  void ReadUnlock();
  void WriteLock();
  void WriteUnlock();

 private:
  std::mutex mutex_;
  std::condition_variable reader_cv_;
  std::condition_variable writer_cv_;
  int active_readers_ = 0;
  int waiting_writers_ = 0;
  int active_writers_ = 0;

  FORBID_COPY_AND_ASSIGN(RWLock);
};

}  // namespace base

#endif  /* BASE_LOCK_ */
