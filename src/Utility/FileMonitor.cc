#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "IO/FileSystemUtils.h"
#include "Base/Log.h"
#include "FileMonitor.h"

namespace Utility {

FileMonitor::~FileMonitor() {
  ShutDown();
  if (monitor_.joinable()) {
    monitor_.join();
  }
}

bool FileMonitor::AddFileToMonitor(const std::string& file_path,
                                   OnFileModifiedCallBack on_file_change_cb) {
  std::string abstract_path = FileSystem::GetAbstractPath(file_path);
  if (abstract_path.empty()) {
    LogERROR("Invalid file path %s, can't add to monitor", file_path.c_str());
    return false;
  }

  if (!FileSystem::FileExists(abstract_path)) {
    LogERROR("File %s doesn't exist, can't add to monitor", file_path.c_str());
    return false;
  }

  // Add to file pool which is being monitored.
  {
    std::unique_lock<std::mutex> lock(files_to_watch_mutex_);
    if (files_to_watch_.find(abstract_path) != files_to_watch_.end()) {
      LogERROR("file %s already being monitored", abstract_path.c_str());
      return false;
    }
    files_to_watch_.emplace(abstract_path,
                            std::unique_ptr<FileToMonitor>(new FileToMonitor(
                                     abstract_path, on_file_change_cb))
                           );
  }

  {
    std::unique_lock<std::mutex> lock(status_mutex_);
    monitor_set_changed = true;
  }
  return true;
}

void FileMonitor::RemoveFileMonitored(const std::string& file_path) {
  std::string abstract_path = FileSystem::GetAbstractPath(file_path);
  if (abstract_path.empty()) {
    LogERROR("Invalid file path %s", file_path.c_str());
    return;
  }

  {
    std::unique_lock<std::mutex> lock(files_to_watch_mutex_);
    auto it = files_to_watch_.find(abstract_path);
    if (it == files_to_watch_.end()) {
      LogINFO("file %s is not being monitored", abstract_path.c_str());
      return;
    }
    files_to_watch_.erase(it);
  }
}

void FileMonitor::ScanMonitoredFiles() {
  std::unique_lock<std::mutex> lock(files_to_watch_mutex_);
  
  for (auto& it: files_to_watch_) {
    auto file_path = it.first;
    auto file_to_monitor = it.second.get();
    struct stat stats;
    if (stat(file_path.c_str(), &stats) != 0) {
      // TODO: Remove the problematic file from monitoring list?
      LogERROR("Can't stat file %s, skip", file_path.c_str());
      continue;
    }
    // File modify time is not changed, don't do anything.
    if (file_to_monitor->last_modify_time == stats.st_mtime) {
      continue;
    }

    // Run the on_file_change callback.
    file_to_monitor->on_file_change_cb(file_path,
                                       file_to_monitor->last_modify_time,
                                       stats.st_mtime);
    file_to_monitor->last_modify_time = stats.st_mtime;
  }

  {
    std::unique_lock<std::mutex> lock(status_mutex_);
    monitor_set_changed = false;
  }
}

void FileMonitor::MonitoringThread() {
  while (true) {
    ScanMonitoredFiles();
    ScanMonitoredDirs();

    std::unique_lock<std::mutex> lock(status_mutex_);
    status_condv_.wait_for(lock, std::chrono::milliseconds(kScanInterval),
                           [this] { return shut_down_ || monitor_set_changed;});
    if (shut_down_) {
      return;
    }
  }
}

void FileMonitor::StartMonitoring() {
  monitor_ = std::thread(&FileMonitor::MonitoringThread, this);
}

void FileMonitor::ShutDown() {
  std::unique_lock<std::mutex> lock(status_mutex_);
  shut_down_ = true;
}

bool FileMonitor::AddDirToMonitor(const std::string& dir_path,
                                  OnFileAddedCallBack on_file_added,
                                  OnFileDeletedCallBack on_file_deleted) {
  std::string abstract_path = FileSystem::GetAbstractPath(dir_path);
  if (abstract_path.empty()) {
    LogERROR("Invalid dir path %s, can't add to monitor", dir_path.c_str());
    return false;
  }

  {
    std::unique_lock<std::mutex> lock(dirs_to_watch_mutex_);
    if (dirs_to_watch_.find(abstract_path) != dirs_to_watch_.end()) {
      LogERROR("dir %s already being monitored", abstract_path.c_str());
      return false;
    }

    dirs_to_watch_.emplace(abstract_path,
                           std::unique_ptr<DirToMonitor>(new DirToMonitor(
                               abstract_path, on_file_added, on_file_deleted)));
    
  }

  {
    std::unique_lock<std::mutex> lock(status_mutex_);
    monitor_set_changed = true;
  }

  return false;
}

void FileMonitor::RemoveDirMonitored(const std::string& dir_path) {
  std::string abstract_path = FileSystem::GetAbstractPath(dir_path);
  if (abstract_path.empty()) {
    LogERROR("Invalid file path %s", dir_path.c_str());
    return;
  }

  {
    std::unique_lock<std::mutex> lock(dirs_to_watch_mutex_);
    auto it = dirs_to_watch_.find(abstract_path);
    if (it == dirs_to_watch_.end()) {
      LogINFO("dir %s is not being monitored", abstract_path.c_str());
      return;
    }
    dirs_to_watch_.erase(it);
  }
}

void FileMonitor::ScanMonitoredDirs() {
  std::unique_lock<std::mutex> lock(dirs_to_watch_mutex_);
  
  for (auto& it: dirs_to_watch_) {
    auto dir_path = it.first;
    auto dir_to_monitor = it.second.get();

    std::vector<std::string> list_files;
    if (FileSystem::ListDir(dir_path, &list_files) < 0) {
      LogERROR("Can't list monitored dir %s", dir_path.c_str());
      return;
    }

    std::set<std::string> crt_files;
    for (auto& file_name: list_files) {
      if (file_name == "." || file_name == "..") {
        continue;
      }
      crt_files.insert(file_name);
      // new file added.
      if (dir_to_monitor->files.find(file_name) == dir_to_monitor->files.end()){
        dir_to_monitor->on_file_added_cb(dir_path, file_name);
      }
    }
    // files deleted.
    for (auto file_name: dir_to_monitor->files) {
      if (crt_files.find(file_name) == crt_files.end()) {
        dir_to_monitor->on_file_deleted_cb(dir_path, file_name);
      }
    }
    dir_to_monitor->files = std::move(crt_files);
  }

  {
    std::unique_lock<std::mutex> lock(status_mutex_);
    monitor_set_changed = false;
  }
}

}  // namespace Utility
