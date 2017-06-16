#include <iostream>

#include "Base/Utils.h"
#include "Base/Log.h"
#include "UnitTest/UnitTest.h"

#include "IO/FileSystemUtils.h"

namespace FileSystem {

class FileSystemTest: public UnitTest {
 public:
  void Test_ListDir() {
    std::vector<std::string> files;
    if (ListDir("test_dir", &files) < 0) {
      LogERROR("Failed to list dir \"test_dir\"");
      return;
    }
    for (const std::string& file : files) {
      printf("%s\n", file.c_str());
    }
  }

  void Test_CleanDir() {
    AssertTrue(CleanDir("test_dir"));
  }
};

}  // namespace Strings

int main() {
  FileSystem::FileSystemTest test;
  test.setup();
  //test.Test_ListDir();
  test.Test_CleanDir();
  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}