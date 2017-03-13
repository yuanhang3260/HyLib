#include <string>

#include "Base/Log.h"
#include "Utility/Timer.h"

namespace Utility {

Timer::~Timer() {
  Stop();
}

bool Timer::Start() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STOPPED) {
      LogERROR("Timer state is not STOPPED, can't Start");
      return false;
    }
    time_out_ = original_time_out_;
    state_ = STARTED;
  }
  runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
  return true;
}

bool Timer::Restart() {
  Stop();

  {
    std::unique_lock<std::mutex> lock(mutex_);
    time_out_ = original_time_out_;
    state_ = STARTED;
  }
  runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
  return true;
}

void Timer::WaitForTimeout() {
  while (true) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      starting_point_ = std::chrono::system_clock::now();
      cv_.wait_for(lock, time_out_,
                   [this] { return state_ == TO_STOP || state_ == TO_PAUSE; });
      if (state_ == TO_STOP) {
        state_ = STOPPED;
        return;
      }
      if (state_ == TO_PAUSE) {
        auto elaped_time = std::chrono::system_clock::now() - starting_point_;
        time_out_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        time_out_ - elaped_time);
        state_ = PAUSED;
        return;
      }
    }
    if (callback_) {
      callback_();
    }

    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (!repeat_) {
        state_ = STOPPED;
        break;
      }
      time_out_ = original_time_out_;
    }
  }
}

bool Timer::Stop() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ == STOPPED || PAUSED) {
      return true;
    }
    state_ = TO_STOP;
  }
  cv_.notify_one();

  if (runner_.joinable()) {
    runner_.join();
  }
  return true;
}

bool Timer::Pause() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STARTED) {
      LogERROR("Timer is not running, can't Pause");
      return false;
    }
    state_ = TO_PAUSE;
  }
  cv_.notify_one();

  if (runner_.joinable()) {
    runner_.join();
  }
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
  runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
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
