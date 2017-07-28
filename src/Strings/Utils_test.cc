#include <string>

#include "Strings/Utils.h"
#include "UnitTest/UnitTest.h"

namespace {
int rd;
}

namespace Strings {

class StringUtilsTest : public UnitTest {
 public:
  void test_Strip() {
    std::cout << __FUNCTION__ << "()..." << std::endl;
    std::string result;
    
    result = Strip("  abcdefg ");
    if (result != "abcdefg") {
      fprintf(stderr, "ERROR in Strip(\"  abcdefg \"), returns \"%s\"\n",
                      result.c_str());
      exit(-1);
    }

    result = Strip("  abcdefg");
    if (result != "abcdefg") {
      fprintf(stderr, "ERROR in Strip(\"  abcdefg\"), returns \"%s\"\n",
                      result.c_str());
      exit(-1);
    }

    result = Strip("abcdefg ");
    if (result != "abcdefg") {
      fprintf(stderr, "ERROR in Strip(\"abcdefg \"), returns \"%s\"\n",
                      result.c_str());
      exit(-1);
    }

    result = Strip("abcdefg", "abfg");
    if (result != "cde") {
      fprintf(stderr,
              "ERROR in Strip(\"abcdefg\", \"abfg\"), returns \"%s\"\n",
              result.c_str());
      exit(-1);
    }

    result = Strip("{}abcdefg}", "{}");
    if (result != "abcdefg") {
      fprintf(stderr,
              "ERROR in Strip(\"{}abcdefg}\", \"{}\"), returns \"%s\"\n",
              result.c_str());
      exit(-1);
    }

    result = Strip("abcde", "abxyzde");
    if (result != "c") {
      fprintf(stderr,
              "ERROR in Strip(\"abcde\", \"abxyzde\"), returns \"%s\"\n",
              result.c_str());
      exit(-1);
    }

    result = Strip("abcdfe", "{abxyzde");
    if (result != "abcdf") {
      fprintf(stderr,
              "ERROR in Strip(\"abcdfe}\", \"abxyzde\"), returns \"%s\"\n",
              result.c_str());
      exit(-1);
    }

    result = Strip(" [abcdfe] ");
    if (result != "[abcdfe]") {
      fprintf(stderr,
              "ERROR in Strip(\" [abcdfe] \", \"[]\"), returns \"%s\"\n",
              result.c_str());
      exit(-1);
    }
  }

  void test_StartWith() {
    std::cout << __FUNCTION__ << "()..." << std::endl;
    std::string str;

    str = "package snoopy;";
    if (!StartWith(str, "package ")) {
      fprintf(stderr,
              "ERROR in StartWith(\"//abcdefg \", \"//\"), should return true\n");
      exit(-1);
    }

    str = "//abcdefg";
    if (StartWith(str, " /")) {
      fprintf(stderr,
              "ERROR in StartWith(\" /abcdefg\", \" /\"), should return false\n");
      exit(-1);
    }

    str = "abc";
    if (!StartWith(str, "")) {
      fprintf(stderr, "ERROR in StartWith(\"abc\", \"\"), should return true\n");
      exit(-1);
    }

    str = "abc";
    if (StartWith(str, "abcdef")) {
      fprintf(stderr,
              "ERROR in StartWith(\"abc\", \"abcdefg\"), should return false\n");
      exit(-1);
    }

    str = "abc";
    if (!StartWith(str, "abc")) {
      fprintf(stderr,
              "ERROR in StartWith(\"abc\", \"abc\"), should return true\n");
      exit(-1);
    }

    str = "";
    if (!StartWith(str, "")) {
      fprintf(stderr,
              "ERROR in StartWith(\"\", \"\"), should return true\n");
      exit(-1);
    }

    str = "";
    if (StartWith(str, "a")) {
      fprintf(stderr,
              "ERROR in StartWith(\"\", \"a\"), should return false\n");
      exit(-1);
    }
  }

  void test_EndWith() {
    std::cout << __FUNCTION__ << "()..." << std::endl;
    std::string str;

    str = "package snoopy;";
    if (!EndWith(str, "snoopy;")) {
      fprintf(stderr,
              "ERROR in EndWith(\"//abcdefg \", \"snoopy;\"), should return true\n");
      exit(-1);
    }

    str = "//abcdefg";
    if (EndWith(str, "g ")) {
      fprintf(stderr,
              "ERROR in EndWith(\" /abcdefg\", \"g \"), should return false\n");
      exit(-1);
    }

    str = "abc";
    if (!EndWith(str, "")) {
      fprintf(stderr, "ERROR in EndWith(\"abc\", \"\"), should return true\n");
      exit(-1);
    }

    str = "abc";
    if (EndWith(str, "abcdef")) {
      fprintf(stderr,
              "ERROR in EndWith(\"abc\", \"abcdefg\"), should return false\n");
      exit(-1);
    }

    str = "abc";
    if (!EndWith(str, "abc")) {
      fprintf(stderr,
              "ERROR in EndWith(\"abc\", \"abc\"), should return true\n");
      exit(-1);
    }

    str = "";
    if (!EndWith(str, "")) {
      fprintf(stderr,
              "ERROR in EndWith(\"\", \"\"), should return true\n");
      exit(-1);
    }

    str = "";
    if (EndWith(str, "a")) {
      fprintf(stderr,
              "ERROR in EndWith(\"\", \"a\"), should return false\n");
      exit(-1);
    }
  }

  void test_ReplaceWith() {
    std::cout << __FUNCTION__ << "()..." << std::endl;
    rd = 0;
    std::string str;

    // Round 0
    {
      str = "ab.cd.efg";
      std::string result = ReplaceWith(str, ".", "::");
      if ("ab::cd::efg" != result) {
        fprintf(stderr,
                "ERROR Round %d ReplaceWith(\"ab.cd.efg \", \".\", \"::\")\n", rd);
        fprintf(stderr, "returns: %s\n", result.c_str());
        exit(-1);
      }
    }

    // Round 1
    {
      str = "ab.cd.efg.";
      std::string result = ReplaceWith(str, ".", "::");
      if ("ab::cd::efg::" != result) {
        fprintf(stderr,
                "ERROR Round %d ReplaceWith(\"ab.cd.efg. \", \".\", \"::\")\n", rd);
        fprintf(stderr, "returns: %s\n", result.c_str());
        exit(-1);
      }
    }

    // Round 2
    {
      str = ".ab.cd.efg";
      std::string result = ReplaceWith(str, ".", "::");
      if ("::ab::cd::efg" != result) {
        fprintf(stderr,
                "ERROR Round %d ReplaceWith(\".ab.cd.efg \", \".\", \"::\")\n", rd);
        fprintf(stderr, "returns: %s\n", result.c_str());
        exit(-1);
      }
    }
  }

  void test_GetToken() {
    std::cout << __FUNCTION__ << "()..." << std::endl;
    rd = 0;
    std::string str;

    // Round 0
    {
      str = "rpc AddStudent(StudentRequest) returns (StudentResponse) {";
      std::vector<std::string> result = ExtractTokens(&str, '(', ')');
      if ("rpc AddStudent returns  {" != str) {
        fprintf(stderr,
                "ERROR Round %d ReplaceWith(rpc AddStudent(StudentRequest) returns (StudentResponse) {", rd);
        fprintf(stderr, "str changed to: %s\n", str.c_str());
        exit(-1);
      }
      if (result.size() != 2) {
        fprintf(stderr,
                "ERROR Round %d ReplaceWith(rpc AddStudent(StudentRequest) returns (StudentResponse) {", rd);
        fprintf(stderr, "expect result.size() = 2, actual: %d\n", (int)result.size());
        exit(-1);
      }
      std::vector<std::string> expect{"(StudentRequest)", "(StudentResponse)"};
      if (result != expect) {
        fprintf(stderr,
                "ERROR Round %d ReplaceWith(rpc AddStudent(StudentRequest) returns (StudentResponse) {", rd);
        fprintf(stderr, "result unmatch\n");
        exit(-1);
      }
      rd++;
    }
  }

  void Test_LongestPrefix() {
    std::cout << __FUNCTION__ << "()..." << std::endl;

    AssertEqual(1, LongestCommonPrefix("ab", "adc"));
    AssertEqual(2, LongestCommonPrefix("ad", "adc"));
    AssertEqual(3, LongestCommonPrefix("adc", "adc"));
    AssertEqual(0, LongestCommonPrefix("ab", ""));

    AssertEqual(0, LongestCommonSuffix("ab", "adc"));
    AssertEqual(1, LongestCommonSuffix("ac", "adc"));
    AssertEqual(3, LongestCommonSuffix("xadc", "adc"));
    AssertEqual(0, LongestCommonSuffix("", "adc"));
  }
};

}  // namespace Strings

int main() {
  Strings::StringUtilsTest test;
  test.setup();

  test.test_Strip();
  test.test_StartWith();
  test.test_EndWith();
  test.test_ReplaceWith();
  test.test_GetToken();
  test.Test_LongestPrefix();

  test.teardown();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}
