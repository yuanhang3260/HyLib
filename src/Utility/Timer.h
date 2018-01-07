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

// Timer triggers a user-specified event on a specified timeout. This class is
// thread-safe.
//
// By default, timer is not set repeating. At repeating mode, it automatically
// restarts after callback is run.
class Timer {
 public:
  enum State {
    STARTED,
    RESTARTED,
    STOPPED,
    PAUSED,
    TERMINATED,
  };

  // Timer state machine:
  //
  //          Start() / Restart()           Resume() / Restart()
  // STOPPED --------------------> STARTED <-------------------- PAUSED -->
  //    ^                           |   |                           ^      |
  //    |                           |   |                           |      |
  //    |           Stop()          |   |          Pause()          |      |
  //     <--------------------------     -------------------------->       |
  //                  ^                                                    |
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
    runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
  }

  ~Timer();

  // Start the timer, will only succeed when timer is in state STOPPED.
  bool Start();
  bool Restart();
  template <class Rep, class Period>
  bool Restart(const std::chrono::duration<Rep, Period>& new_time_out) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      original_time_out_ = new_time_out;
      time_out_ = original_time_out_;
      state_ = RESTARTED;
    }
    run_cv_.notify_one();
    timeout_cv_.notify_one();
    return true;
  }

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
  std::condition_variable run_cv_;
  std::condition_variable timeout_cv_;
  State state_ = STOPPED;

  std::thread runner_;

  std::chrono::time_point<std::chrono::system_clock> starting_point_;

  bool repeat_ = false;
};

}

#endif  // UTILITY_TIMER_
