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

// This a timer which triggers a user-specified event on timeout. This class is
// thread-safe.
//
// By default, timer is not set in repeating mode. In repeating mode, the timer
// automatically restart after callback is done.
class Timer {
 public:
  enum State {
    STARTED,
    TO_STOP,
    STOPPED,
    TO_PAUSE,
    PAUSED,
  };

  // Timer state machine:
  //
  //          Start() / Restart()           Resume() / Restart()
  // STOPPED --------------------> STARTED <-------------------- PAUSED -->
  //    ^                           |   |                           ^      |
  //    |                           |   |                           |      |
  //    |                           |   |                           |      |
  //     <-------- TO_STOP <--------     --------> TO_PAUSE ------->       |
  //        wake      ^       Stop()      Pause()             wake         |
  //                  |                                                    |
  //                  |                                                    |
  //                   <---------------- <---------------- <---------------
  //                                          Stop()
  //

  template <class Rep, class Period>
  Timer(const std::chrono::duration<Rep, Period>& rel_time,
        std::function<void()> cb) :
      time_out_(std::chrono::duration_cast<std::chrono::nanoseconds>(rel_time)),
      original_time_out_(time_out_),          
      callback_(std::move(cb)) {
  }

  ~Timer();

  // Start the timer, will only succeed when timer is in state STOPPED.
  bool Start();
  bool Restart();

  bool Stop();

  // Pause the timer, will only succeed when timer is in state STARTED.
  bool Pause();
  // Resume the timer, will only succeed when timer is in state PAUSED.
  bool Resume();

  void SetRepeat(bool repeat);

  State state();

 private:
  void WaitForTimeout();

  std::chrono::nanoseconds time_out_;
  std::chrono::nanoseconds original_time_out_;
  std::function<void()> callback_;

  std::mutex mutex_;
  std::condition_variable cv_;
  State state_ = STOPPED;

  std::thread runner_;

  std::chrono::time_point<std::chrono::system_clock> starting_point_;

  bool repeat_ = false;
};

}

#endif  // UTILITY_TIMER_
