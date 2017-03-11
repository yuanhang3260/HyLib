#include <string>

#include "Base/Log.h"
#include "Utility/Timer.h"

namespace Utility {

template <class Rep, class Period>
Timer::Timer(const std::chrono::duration<Rep,Period>& rel_time,
             std::function<void()> cb) :
    time_out_(std::chrono::duration_cast<std::chrono::nanoseconds>(rel_time)),
    callback_(cb) {
}

Timer::~Timer() {
  Stop();
}

void Timer::Start() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STOPPED) {
      LogERROR("Timer state is not STOPPED, can't Start");
      return;
    }
    state_ = STARTED;
  }
  runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
}

void Timer::Restart() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    state_ = STOPPED;
  }
  cv_.notify_one();

  if (runner_.joinable()) {
    runner_.join();
  }
  runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
}

void Timer::WaitForTimeout() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    starting_point_ = std::chrono::system_clock::now();
    cv_.wait_for(lock, time_out_,
                 [this] { return state_ == STOPPED || state_ == PAUSED; });
    if (state_ == STOPPED) {
      return;
    }
    if (state_ == PAUSED) {
      auto elaped_time = std::chrono::system_clock::now() - starting_point_;
      time_out_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
          time_out_ - elaped_time);
      return;
    }
  }
  if (callback_) {
    callback_();
  }
}

void Timer::Stop() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STARTED) {
      LogERROR("Timer is not running, can't Stop");
      return;
    }
    state_ = STOPPED;
  }
  cv_.notify_one();

  if (runner_.joinable()) {
    runner_.join();
  }
}

void Timer::Pause() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != STARTED) {
      LogERROR("Timer is not running, can't Pause");
      return;
    }
    state_ = PAUSED;
  }
  cv_.notify_one();

  if (runner_.joinable()) {
    runner_.join();
  }
}

void Timer::Resume() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (state_ != PAUSED) {
      LogERROR("Current time state is not PAUSED, can't Resume");
      return;
    }
  }
  runner_ = std::thread(std::bind(&Timer::WaitForTimeout, this));
}

}  // namespace Utility
