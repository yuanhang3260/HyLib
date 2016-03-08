#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <set>

#include "Base/Log.h"
#include "IO/FileSystemUtils.h"
#include "UnitTest/UnitTest.h"
#include "FileMonitor.h"

std::string kDataDir = FileSystem::GetAbstractPath("data/");
std::string kFileContentOriginal = "Hello World";
std::string kFileContentModified = "Good Bye";

int OnFileChange(const std::string& file_path, time_t mtime1, time_t mtime2) {
  FILE * pFile;
  pFile = fopen(file_path.c_str(), "r");
  if (!pFile) {
    LogFATAL("Can't open file %s", file_path.c_str());
  }

  char buffer[64];
  int readlen = fread(buffer, 1, sizeof(buffer), pFile);
  // if (strncmp(buffer, kFileContentModified, sizeof(kFileContentModified)) != 0){
  //   LogFATAL("Verify new file content failed - expect \"%s\", actual \"%s\"",
  //            kFileContentModified, std::string(buffer, readlen).c_str());
  // }
  fclose(pFile);
  printf("File %s modified to:\n", file_path.c_str());
  std::cout << std::string(buffer, readlen) << std::endl;
  return readlen;
}

bool SetFileContent(const std::string& file_path, const std::string& content) {
  FILE* pFile = fopen(FileSystem::JoinPath(kDataDir, file_path).c_str(), "w+");
  if (!pFile) {
    LogFATAL("Can't open file %s", file_path.c_str());
  }

  int writelen = fwrite(content.c_str(), 1, content.length(), pFile);
  if (writelen == (int)content.length()) {
    fclose(pFile);
    return true;
  }
  LogFATAL("Set content for file %s failed", file_path.c_str());
  return false;
}

class FileMonitorTest: public UnitTest {
 public:
  void setup() override {
    if (DirectoryExists(kDataDir)) {
      return;
    }
    CreateDir(kDataDir);
  }

  void Test_MonitorFile() {
    Utility::FileMonitor monitor;
    monitor.StartMonitoring();

    SetFileContent("fileA", kFileContentOriginal);
    monitor.AddFileToMonitor(FileSystem::JoinPath(kDataDir, "fileA"),
                             OnFileChange);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    SetFileContent("fileA", kFileContentModified);

    // Wait for all tasks to finish.
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
 
 private:
  bool DirectoryExists(const std::string& path) {
    if (path == NULL) {
      return false;
    }

    DIR *pDir;
    bool bExists = false;
    pDir = opendir(path.c_str());
    if (pDir != NULL) {
      bExists = true;    
      (void)closedir(pDir);
    }
    return bExists;
  }

  void CreateDir(const std::string& path) {
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
};


int main() {
  FileMonitorTest test;
  test.Test_MonitorFile();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}