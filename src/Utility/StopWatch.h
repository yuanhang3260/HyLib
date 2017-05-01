#ifndef UTILITY_STOPWATCH_
#define UTILITY_STOPWATCH_

#include <atomic>
#include <chrono>
#include <mutex>

#include "Base/BaseTypes.h"

namespace Utility {

// This class is thread-safe.
class StopWatch {
 public:
  StopWatch();

  // StopWatch must be launched with Start(), and after that, Start() works
  // samely as Resume() is stopwatch is paused.
  bool Start();
  bool Pause();
  bool Resume();
  bool Reset();

  std::chrono::nanoseconds elapsed_time();

  enum State {
    STOPPED,
    RUNNING,
    PAUSED,
  };

 private:
  std::chrono::time_point<std::chrono::system_clock> last_start_;
  std::chrono::nanoseconds elapsed_time_;
  State state_ = STOPPED;
  std::mutex mutex_;
};


}  // namespace Utility

#endif  // UTILITY_STOPWATCH_