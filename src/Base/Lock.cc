#include "Base/Lock.h"

namespace base {

void RWLock::ReadLock() {
  std::unique_lock<std::mutex> lock(mutex_);
  reader_cv_.wait(lock, [&] { return waiting_writers_ == 0 &&
                                     active_writers_ == 0; });
  active_readers_++;
}

void RWLock::ReadUnlock() {
  std::unique_lock<std::mutex> lock(mutex_);
  active_readers_--;
  if (active_readers_ == 0) {
    writer_cv_.notify_one();
  }
}

void RWLock::WriteLock() {
  std::unique_lock<std::mutex> lock(mutex_);
  waiting_writers_++;
  writer_cv_.wait(lock, [&] { return active_readers_ == 0 &&
                                     active_writers_ == 0; });
  waiting_writers_--;
  active_writers_++;
}

void RWLock::WriteUnlock() {
  std::unique_lock<std::mutex> lock(mutex_);
  waiting_writers_--;
  active_writers_--;
  if (waiting_writers_ > 0) {
    writer_cv_.notify_one();
  }
  else {
    reader_cv_.notify_all();
  }
}

}  // namespace base