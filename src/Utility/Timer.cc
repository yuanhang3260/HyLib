#include <string>

#include "Base/Log.h"
#include "Utility/Timer.h"

namespace Utility {

Timer::~Timer() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    state_ = TERMINATED;
  }
  run_cv_.notify_one();
  timeout_cv_.notify_one();

  if (runner_.joinable()) {
    runner_.join();
  }
}

bool Timer::Start() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STOPPED) {
      LogERROR("Timer state is not STOPPED, can't Start");
      return false;
    }
    state_ = STARTED;
  }
  run_cv_.notify_one();
  return true;
}

bool Timer::Restart() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    time_out_ = original_time_out_;
    state_ = RESTARTED;
  }
  run_cv_.notify_one();
  timeout_cv_.notify_one();
  return true;
}

void Timer::WaitForTimeout() {
  while (true) {
    {
      // Only continue if state is STARTED or RESTARTED.
      std::unique_lock<std::mutex> lock(mutex_);
      run_cv_.wait(lock, [this] { return state_ == STARTED ||
                                         state_ == RESTARTED ||
                                         state_ == TERMINATED; });
      if (state_ == TERMINATED) {
        break;
      }

      if (state_ == STOPPED || state_ == PAUSED) {
        continue;
      }

      if (state_ == RESTARTED) {
        state_ = STARTED;
      }

      // Wait for timeout.
      starting_point_ = std::chrono::system_clock::now();
      timeout_cv_.wait_for(lock, time_out_,
                          [this] { return state_ == STOPPED ||
                                          state_ == PAUSED ||
                                          state_ == RESTARTED ||
                                          state_ == TERMINATED; });
      if (state_ == TERMINATED) {
        break;
      }

      if (state_ == STOPPED || state_ == RESTARTED) {
        continue;
      }
      if (state_ == PAUSED) {
        auto elaped_time = std::chrono::system_clock::now() - starting_point_;
        time_out_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        time_out_ - elaped_time);
        continue;
      }
    }
    if (callback_) {
      callback_();
    }

    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (!repeat_) {
        state_ = STOPPED;
      }
      time_out_ = original_time_out_;
    }
  }
}

bool Timer::Stop() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ == STOPPED) {
      return true;
    }
    time_out_ = original_time_out_;
    state_ = STOPPED;
  }
  timeout_cv_.notify_one();
  return true;
}

bool Timer::Pause() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STARTED) {
      LogERROR("Timer is not running, can't Pause");
      return false;
    }
    state_ = PAUSED;
  }
  timeout_cv_.notify_one();
  return true;
}

bool Timer::Resume() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != PAUSED) {
      LogERROR("Current time state is not PAUSED, can't Resume");
      return false;
    }
    state_ = STARTED;
  }
  run_cv_.notify_one();
  return true;
}

void Timer::SetRepeat(bool repeat) {
  std::unique_lock<std::mutex> lock(mutex_);
  repeat_ = repeat;
}

Timer::State Timer::state() {
  std::unique_lock<std::mutex> lock(mutex_);
  return state_;
}

}  // namespace Utility
