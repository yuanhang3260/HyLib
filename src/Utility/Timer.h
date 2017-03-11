#ifndef UTILITY_TIMER_
#define UTILITY_TIMER_

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "Base/BaseTypes.h"

namespace Utility {

// This a timer which triggers a user-specified event in timeout. This class is
// NOT thread-safe.
class Timer {
 public:
  template <class Rep, class Period>
  Timer(const std::chrono::duration<Rep, Period>& rel_time,
        std::function<void()> cb);
  ~Timer();

  void Start();
  void Restart();

  void Stop();
  void Pause();
  void Resume();

 private:
  enum State {
    STARTED,
    STOPPED,
    PAUSED,
  };

  void WaitForTimeout();

  std::chrono::nanoseconds time_out_;
  std::function<void()> callback_;

  std::mutex mutex_;
  std::condition_variable cv_;
  State state_ = STOPPED;

  std::thread runner_;

  std::chrono::time_point<std::chrono::system_clock> starting_point_;
};

}

#endif  // UTILITY_TIMER_