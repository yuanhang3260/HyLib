#include <iostream>

#include "Base/Utils.h"
#include "Base/Log.h"
#include "UnitTest/UnitTest.h"

#include "IO/FileSystemUtils.h"

namespace FileSystem {

class FileSystemTest: public UnitTest {
 public:
  void Test_FileSystemUtils() {
    AssertTrue(RenameFile("old", "new"));    
  }
};

}  // namespace Strings

int main() {
  FileSystem::FileSystemTest test;
  test.setup();
  test.Test_FileSystemUtils();
  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}