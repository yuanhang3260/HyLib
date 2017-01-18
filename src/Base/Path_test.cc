#include <vector>
#include <set>

#include "UnitTest/UnitTest.h"
#include "Base/Path.h"

class PathTest: public UnitTest {
 public:
  void Test_JoinPath() {
    std::string expect_result = "/usr/local/bin/foo";
    AssertEqual(expect_result,
                Path::JoinPath("/usr", "/local", "/bin/", "foo/"));

    expect_result = "usr/local/bin/foo";
    std::string str1 = "usr";
    std::string str2 = "/bin/";
    AssertEqual(expect_result,
                Path::JoinPath(str1, "/local", str2, "foo/"));
  }
};


int main() {
  PathTest test;
  test.Test_JoinPath();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}