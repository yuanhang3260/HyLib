// FileMonitor class monitors files or direcotories changes and accept user 
// defined callback to execute when file contents are updates. This is typically
// used for monitoring such as config files, data input files.
//
// This class is thread-safe.

#ifndef UTILITY_FILE_MONITOR_
#define UTILITY_FILE_MONITOR_

#include <string>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include <mutex>

#include "Utility/ThreadPool.h"

using OnFileModifiedCallBack =
          std::function<void(const std::string&, time_t, time_t)>;

class FileMonitor {
 public:
  FileMonitor() = default;
  virtual ~FileMonitor();

  // Add file or directory to monitor.
  bool AddFileToMonitor(const std::string& file_path,
                        OnFileModifiedCallBack on_file_change_cb);
  bool AddDirToMonitor(const std::string& dir_path);

  // Start monitoring thread.
  void StartMonitoring();

  // Set thread pool size.
  void SetThreadPoolSize(int size);

 private:
  const int kScanInterval = 500;  // milliseconds

  class FileToMonitor {
   public:
    FileToMonitor(const std::string& filepath, OnFileModifiedCallBack cb) :
        file_path(filepath),
        on_file_change_cb(cb) {
    }

    std::string file_path;
    OnFileModifiedCallBack on_file_change_cb;
    time_t last_modify_time;
  };

  // Scan all files being monitored.
  void ScanMonitoredFiles();

  // File monitoring thread.
  void MonitoringThread();

  std::map<std::string, std::unique_ptr<FileToMonitor>> files_to_watch_;
  std::mutex files_to_watch_mutex_;

  std::set<std::string> dirs_to_watch_;
  std::mutex dirs_to_watch_mutex_;

  Executors::FixedThreadPool thread_pool_;
};

#endif  /* UTILITY_FILE_MONITOR_ */
