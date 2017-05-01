#include "Utility/StopWatch.h"

namespace Utility {

StopWatch::StopWatch() :
    elapsed_time_(std::chrono::nanoseconds(0)),
    state_(STOPPED) {
}

bool StopWatch::Start() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (state_ != STOPPED && state_ != PAUSED) {
    return false;
  }

  last_start_ = std::chrono::system_clock::now();
  state_ = RUNNING;
  return true;
}

bool StopWatch::Pause() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (state_ != RUNNING) {
    return false;
  }

  auto now = std::chrono::system_clock::now();
  elapsed_time_ += (now - last_start_);

  state_ = PAUSED;
  return true;
}

bool StopWatch::Resume() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (state_ != PAUSED) {
    return false;
  }

  last_start_ = std::chrono::system_clock::now();
  state_ = RUNNING;
  return true;
}

bool StopWatch::Reset() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (state_ != PAUSED) {
    return false;
  }

  elapsed_time_ = std::chrono::nanoseconds(0);
  state_ = STOPPED;
  return true;
}

std::chrono::nanoseconds StopWatch::elapsed_time() {
  std::unique_lock<std::mutex> lock(mutex_);
  return elapsed_time_;
}

}  // namespace Utility
