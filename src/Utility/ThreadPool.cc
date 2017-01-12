#include <iostream>

#include "Base/Log.h"
#include "Base/Utils.h"
#include "ThreadPool.h"

namespace Executors {

FixedThreadPool::FixedThreadPool(): 
    state_(IDLE),
    pool_size(4) {
}

// Constructor does nothing. Threads are created when new task submitted.
FixedThreadPool::FixedThreadPool(size_t num_threads):
    state_(IDLE),
    pool_size(num_threads) {
}

void FixedThreadPool::SetPoolSize(size_t size) {
  pool_size = size;
}

size_t FixedThreadPool::Size() const {
  return pool_size;
}

// Destructor joins all threads
FixedThreadPool::~FixedThreadPool() {
  for(std::thread &worker: workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void FixedThreadPool::Start() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    state_ = RUNNING;
    int num_working_threads_ = Utils::Min(pool_size, tasks.size());
    for (int i = workers.size(); i < num_working_threads_; i++) {
      workers.emplace_back(std::thread(&FixedThreadPool::ThreadWorker, this));
    }
  }
  condition.notify_all();
}

// Thread worker
void FixedThreadPool::ThreadWorker() {
  Executors::Closure task;
  while (1) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      condition.wait(lock,
                     [this] { return state_ == STOP || !tasks.empty(); });
      if (state_ == STOP && tasks.empty()) {
        return;
      }
      task = tasks.front();
      tasks.pop();
    }
    task();
  }
}

// Add new work item to the pool.
void FixedThreadPool::AddTask(Executors::Closure task) {
  {
    std::unique_lock<std::mutex> lock(worker_mutex);
    if (state_ == RUNNING && workers.size() < pool_size) {
      workers.emplace_back(std::thread(&FixedThreadPool::ThreadWorker, this));
    }
  }

  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (state_ == STOP) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks.emplace(std::move(task));
  }
  condition.notify_one();
}

// Blocks and wait for all previously submitted tasks to be completed.
void FixedThreadPool::AwaitTermination() {
  condition.notify_all();
  for(std::thread &worker: workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

// Shut down the threadpool. This method does not wait for previously submitted
// tasks to be completed.
void FixedThreadPool::Stop() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    state_ = STOP;
  }
  condition.notify_all();
}

}  // namespace Executors
