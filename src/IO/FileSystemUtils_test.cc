#include <vector>
#include <set>

#include "UnitTest/UnitTest.h"
#include "FileSystemUtils.h"

class FileSystemUtilsTest: public UnitTest {
 public:
  void Test_JoinPath() {
    std::string expect_result = "/usr/local/bin/foo";
    AssertEqual(expect_result,
                FileSystem::JoinPath("usr", "/local", "/bin/", "foo/"));

    std::string str1 = "usr";
    std::string str2 = "/bin/";
    AssertEqual(expect_result,
                FileSystem::JoinPath(str1, "/local", str2, "foo/"));
  }
};


int main() {
  FileSystemUtilsTest test;
  test.Test_JoinPath();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}