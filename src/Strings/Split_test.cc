#include <iostream>

#include "Base/Utils.h"
#include "Base/Log.h"
#include "UnitTest/UnitTest.h"

#include "Strings/Split.h"

namespace Strings {

class SplitTest: public UnitTest {
 public:
  void Test_Split() {
    auto re = Split("a,b,c,d", ',');
    AssertEqual(4, re.size());

    re = Split("a,b,c,d,,", ",");
    AssertEqual(6, re.size());
    AssertTrue(std::vector<std::string>{"a", "b", "c", "d", "", ""} == re);

    re = Split(",a,,b,c,d", ",");
    AssertEqual(6, re.size());
    AssertTrue(std::vector<std::string>{"", "a", "", "b", "c", "d"} == re);

    re = Split("abcd,", "");
    AssertEqual(5, re.size());
    AssertTrue(std::vector<std::string>{"a", "b", "c", "d", ","} == re);

    re = Split("", "c");
    AssertEqual(1, re.size());
    AssertTrue(std::vector<std::string>{""} == re);

    re = Split("a=1&b=2;c=3;", delimiters::AnyOf("&;"));
    AssertEqual(4, re.size());
    AssertTrue(std::vector<std::string>{"a=1", "b=2", "c=3", ""} == re);

    re = Split("a=1&b=2;c=3;; ", delimiters::AnyOf("&;"), Strings::SkipEmpty());
    AssertEqual(4, re.size());
    AssertTrue(std::vector<std::string>{"a=1", "b=2", "c=3", " "} == re);

    re = Split("a=1&b=2;c=3; ;\t;", delimiters::AnyOf("&;"),
               Strings::SkipWhiteSpace());
    AssertEqual(3, re.size());
    AssertTrue(std::vector<std::string>{"a=1", "b=2", "c=3"} == re);
  }
};

}  // namespace Strings

int main() {
  Strings::SplitTest test;
  test.setup();
  test.Test_Split();
  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}