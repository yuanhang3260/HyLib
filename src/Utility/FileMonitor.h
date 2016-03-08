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
#include <thread>
#include <mutex>
#include <condition_variable>

using OnFileModifiedCallBack =
          std::function<void(const std::string&, time_t, time_t)>;

namespace Utility {

class FileMonitor {
 public:
  FileMonitor() = default;
  virtual ~FileMonitor();

  // Add/Remove file to monitor.
  bool AddFileToMonitor(const std::string& file_path,
                        OnFileModifiedCallBack on_file_change_cb);
  void RemoveFileMonitored(const std::string& file_path);

  // Add/Remove directory to monitor.
  bool AddDirToMonitor(const std::string& dir_path);

  // Start monitoring thread.
  void StartMonitoring();

  // Shutdown.
  void ShutDown();

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
    time_t last_modify_time = 0;
  };

  // Scan all files being monitored.
  void ScanMonitoredFiles();

  // File monitoring thread.
  void MonitoringThread();

  std::map<std::string, std::unique_ptr<FileToMonitor>> files_to_watch_;
  std::mutex files_to_watch_mutex_;

  std::set<std::string> dirs_to_watch_;
  std::mutex dirs_to_watch_mutex_;

  bool shut_down_ = false;
  bool new_added_ = false;
  std::mutex status_mutex_;
  std::condition_variable status_condv_;

  std::thread monitor_;
};

}  // namespace Utility

#endif  /* UTILITY_FILE_MONITOR_ */
