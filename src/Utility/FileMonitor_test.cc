#include <string.h>
#include <iostream>
#include <vector>
#include <set>

#include "Base/Log.h"
#include "IO/File.h"
#include "IO/FileSystemUtils.h"
#include "UnitTest/UnitTest.h"
#include "FileMonitor.h"

std::string kDataDir = "data/";
std::string kFileContentOriginal = "Hello World";
std::string kFileContentModified = "Good Bye";

void OnFileChange(const std::string& file_path, time_t mtime1, time_t mtime2) {
  std::string content;
  File::GetContent(file_path, &content);

  printf("File %s modified to:\n", file_path.c_str());
  std::cout << content << std::endl;
}

void OnFileAdded(const std::string& dir_path, const std::string& file_path) {
  printf("File \"%s\" added\n",
         FileSystem::JoinPath(dir_path, file_path).c_str());
}

void OnFileDeleted(const std::string& dir_path, const std::string& file_path) {
  printf("File \"%s\" deleted\n",
         FileSystem::JoinPath(dir_path, file_path).c_str());
}

class FileMonitorTest: public UnitTest {
 public:
  void setup() override {
    if (FileSystem::DirectoryExists(kDataDir)) {
      return;
    }
    FileSystem::CreateDir(kDataDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }

  void Test_MonitorFile() {
    Utility::FileMonitor monitor;
    monitor.StartMonitoring();


    // Create a file and monitor it.
    File::SetContent(kDataDir + "fileA", kFileContentOriginal);
    monitor.AddFileToMonitor(FileSystem::JoinPath(kDataDir, "fileA"),
                             OnFileChange);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Change file contents.
    File::SetContent(kDataDir + "/fileA", kFileContentModified);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Creat dir to monitor.
    AssertTrue(FileSystem::CreateDir(kDataDir + "/tmp",
                                     S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
    monitor.AddDirToMonitor(FileSystem::JoinPath(kDataDir, "tmp"),
                            OnFileAdded, OnFileDeleted);

    // Add two files into dir.
    FileSystem::CreateFile(FileSystem::JoinPath(kDataDir, "tmp", "foo"));
    FileSystem::CreateFile(FileSystem::JoinPath(kDataDir, "tmp", "bar"));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Remove one file from the dir. Expecting monitor printing info.
    FileSystem::Remove(FileSystem::JoinPath(kDataDir, "tmp", "bar"));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Stop monitoring this dir, and remove the other file in it
    // Nothing should happen.
    monitor.RemoveDirMonitored(kDataDir + "/tmp");
    FileSystem::Remove(FileSystem::JoinPath(kDataDir, "tmp", "foo"));

    // Wait for all tasks to finish.
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};


int main() {
  FileMonitorTest test;
  test.setup();
  test.Test_MonitorFile();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}