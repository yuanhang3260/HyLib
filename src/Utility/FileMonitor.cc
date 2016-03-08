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
    new_added_ = true;
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
      LogERROR("file %s already being monitored", abstract_path.c_str());
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
    new_added_ = false;
  }
}

void FileMonitor::MonitoringThread() {
  while (true) {
    ScanMonitoredFiles();

    std::unique_lock<std::mutex> lock(status_mutex_);
    status_condv_.wait_for(lock, std::chrono::milliseconds(kScanInterval),
                           [this] { return shut_down_ || new_added_; });
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

bool FileMonitor::AddDirToMonitor(const std::string& dir_path) {
  std::string abstract_dir_path = FileSystem::GetAbstractPath(dir_path);
  if (abstract_dir_path.empty()) {
    LogERROR("Invalid dir path %s, can't add to monitor", dir_path.c_str());
    return false;
  }

  {
    std::unique_lock<std::mutex> lock(dirs_to_watch_mutex_);
    if (dirs_to_watch_.find(abstract_dir_path) != dirs_to_watch_.end()) {
      LogERROR("dir %s already being monitored", abstract_dir_path.c_str());
      return false;
    }

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(abstract_dir_path.c_str())) != nullptr) {
      dirs_to_watch_.insert(abstract_dir_path);
      // print all the files and directories within directory.
      while ((ent = readdir(dir)) != NULL) {
        //AddFileToMonitor(FileSystem::JoinPath(abstract_dir_path, ent->d_name));
      }
      closedir(dir);
      return true;
    }
  }
  
  LogERROR("Can't list files in directory %s", dir_path.c_str());
  return false;
}


}  // namespace Utility
