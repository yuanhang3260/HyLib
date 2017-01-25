#include <iostream>

#include "Base/Log.h"
#include "Base/Utils.h"
#include "ThreadPool.h"

namespace Executors {

FixedThreadPool::FixedThreadPool(): 
    state_(IDLE),
    pool_size_(4) {
}

// Constructor does nothing. Threads are created when new task submitted.
FixedThreadPool::FixedThreadPool(size_t num_threads):
    state_(IDLE),
    pool_size_(num_threads) {
}

void FixedThreadPool::SetPoolSize(size_t size) {
  pool_size_ = size;
}

size_t FixedThreadPool::Size() const {
  return pool_size_;
}

// Destructor joins all threads
FixedThreadPool::~FixedThreadPool() {
  for(std::thread &worker: workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void FixedThreadPool::Start() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    state_ = RUNNING;
    int num_working_threads_ = Utils::Min(pool_size_, tasks_.size());
    for (int i = workers_.size(); i < num_working_threads_; i++) {
      workers_.emplace_back(std::thread(&FixedThreadPool::ThreadWorker, this));
    }
  }
  condition_.notify_all();
}

// Thread worker
void FixedThreadPool::ThreadWorker() {
  Executors::Closure task;
  while (1) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      condition_.wait(lock,
                     [this] { return state_ == STOP || !tasks_.empty(); });
      if (state_ == STOP && tasks_.empty()) {
        return;
      }
      task = tasks_.front();
      tasks_.pop();
    }
    task();
  }
}

// Add new work item to the pool.
void FixedThreadPool::AddTask(Executors::Closure task) {
  {
    std::unique_lock<std::mutex> lock(worker_mutex_);
    if (state_ == RUNNING && workers_.size() < pool_size_) {
      workers_.emplace_back(std::thread(&FixedThreadPool::ThreadWorker, this));
    }
  }

  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    if (state_ == STOP) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks_.emplace(std::move(task));
  }
  condition_.notify_one();
}

// Blocks and wait for all previously submitted tasks to be completed.
void FixedThreadPool::AwaitTermination() {
  condition_.notify_all();
  for(std::thread &worker: workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

// Shut down the threadpool. This method does not wait for previously submitted
// tasks to be completed.
void FixedThreadPool::Stop() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    state_ = STOP;
  }
  condition_.notify_all();
}

}  // namespace Executors
