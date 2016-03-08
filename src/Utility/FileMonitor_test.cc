#include <string.h>
#include <iostream>
#include <vector>
#include <set>

#include "Base/Log.h"
#include "IO/File.h"
#include "IO/FileSystemUtils.h"
#include "UnitTest/UnitTest.h"
#include "FileMonitor.h"

std::string kDataDir = FileSystem::GetAbstractPath("data/");
std::string kFileContentOriginal = "Hello World";
std::string kFileContentModified = "Good Bye";

void OnFileChange(const std::string& file_path, time_t mtime1, time_t mtime2) {
  std::string content;
  File::GetContent(file_path, &content);

  printf("File %s modified to:\n", file_path.c_str());
  std::cout << content << std::endl;
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

    File::SetContent(kDataDir + "/fileA", kFileContentOriginal);
    monitor.AddFileToMonitor(FileSystem::JoinPath(kDataDir, "fileA"),
                             OnFileChange);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    File::SetContent(kDataDir + "/fileA", kFileContentModified);

    // Wait for all tasks to finish.
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};


int main() {
  FileMonitorTest test;
  test.Test_MonitorFile();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}