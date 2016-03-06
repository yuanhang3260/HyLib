#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "TextPrinter.h"

const char* outputfile;
int rd = 0;

void test_PrintVectorMatch_Impl(
    std::string content,
    std::vector<std::string> matches,
    std::string result)
{
  std::string outname = std::string(outputfile);
  IO::TextPrinter printer(outname);
  printer.Print(content, matches);
  printer.Flush();
  FILE* file = fopen(outputfile, "r");
  char buf[256];
  memset(buf, 0, 256);
  int nread = fread(buf, 1, sizeof(buf), file);
  if (strncmp((const char*)buf, result.c_str(), result.length()) != 0) {
    fprintf(stderr, "ERROR: test_PrintVectorMatch - round %d\n", rd);
    fprintf(stderr, "       expect: %s\n       actual: %s, nread = %d\n",
            result.c_str(), buf, nread);
    exit(-1);
  }
  rd++;
  fclose(file);
}

void test_PrintVectorMatch() {
  std::cout << __FUNCTION__ << " ..." << std::endl;
  rd = 0;

  // Round 0
  {
    std::vector<std::string> matches;
    std::string content("xxxx$yyy$zzzzz$wwww$uuuu");
    std::string result("xxxxyyyzzzzzwwwwuuuu");
    test_PrintVectorMatch_Impl(content, matches, result);
  }

  // Round 1
  {
    std::vector<std::string> matches = {"Match1", "Match2", "Match3", "Match4"};
    std::string content("xxxx$yyy$zz\nzzz$wwww$uuuu");
    std::string result("xxxxMatch1yyyMatch2zz\nzzzMatch3wwwwMatch4uuuu");
    test_PrintVectorMatch_Impl(content, matches, result);
  }

  // Round 2
  {
    std::vector<std::string> matches = {"Match1", "Match2", "Match3"};
    std::string content = "xxxx$yyy$zz\nzzz$wwww$uuuu";
    std::string result("xxxxMatch1yyyMatch2zz\nzzzMatch3wwwwuuuu");
    test_PrintVectorMatch_Impl(content, matches, result);
  }

  // Round 3
  {
    std::vector<std::string> matches = {"Match1", "Match2", "Match3", "Match4"};
    std::string content("$xxxxyyy$zz\nzzz");
    std::string result("Match1xxxxyyyMatch2zz\nzzz");
    test_PrintVectorMatch_Impl(content, matches, result);
  }

  // Round 4
  {
    std::vector<std::string> matches = {"Match1", "Match2", "Match3", "Match4"};
    std::string content("xxxx$yyy$zz\nzzzwwwwuuuu$");
    std::string result("xxxxMatch1yyyMatch2zz\nzzzwwwwuuuuMatch3");
    test_PrintVectorMatch_Impl(content, matches, result);
  }

  // Round 5
  {
    std::vector<std::string> matches = {"Match1", "Match2", "Match3"};
    std::string content("xxxx$zzzzzwwwwuuuu$$$vv");
    std::string result("xxxxMatch1zzzzzwwwwuuuu$Match2vv");
    test_PrintVectorMatch_Impl(content, matches, result);
  }

  std::cout << "Passed ^_^\n" << std::endl;
}

void test_PrintMapMatch_Impl(
    std::string content,
    std::map<std::string, std::string> matches,
    std::string result)
{
  std::string outname = std::string(outputfile);
  IO::TextPrinter printer(outname);
  printer.Print(content, matches);
  printer.Flush();
  FILE* file = fopen(outputfile, "r");
  char buf[256];
  memset(buf, 0, 256);
  int nread = fread(buf, 1, sizeof(buf), file);
  if (strncmp((const char*)buf, result.c_str(), result.length()) != 0) {
    fprintf(stderr, "ERROR: test_PrintMapMatch - round %d\n", rd);
    fprintf(stderr, "       expect: %s\n       actual: %s, nread = %d\n",
            result.c_str(), buf, nread);
    exit(-1);
  }
  rd++;
  fclose(file);
}

void test_PrintMapMatch() {
  std::cout << __FUNCTION__ << " ..." << std::endl;
  rd = 0;
  
  // Round 0
  {
    std::map<std::string, std::string> matches = {
      {"Key1", "Match1"},
      {"Key2", "Match2"},
      {"Key3", "Match3"},
    };
    std::string content =
        "xxxx${Key1}yyy${Key2}zz\nzzz${Key3}wwww${Invalid_Key}";
    std::string result("xxxxMatch1yyyMatch2zz\nzzzMatch3wwww");
    test_PrintMapMatch_Impl(content, matches, result);
  }

  // Round 1
  {
    std::map<std::string, std::string> matches = {
      {"Key1", "Match1"},
      {"Key2", "Match2"},
      {"Key3", "Match3"},
    };
    std::string content = "xxxx${Key1}yyy${}zzzzz${Key3wwww";
    std::string result("xxxxMatch1yyyzzzzz");
    test_PrintMapMatch_Impl(content, matches, result);
  }

  // Round 2
  {
    std::map<std::string, std::string> matches = {
      {"{Key1", "Match1"},
      {"Key2", "Match2"},
      {"Key3", "Match3"},
    };
    std::string content = "xxxx${{Key1}}yyy${Key2}zzzzz${Key3}wwww}";
    std::string result("xxxxMatch1}yyyMatch2zzzzzMatch3wwww}");
    test_PrintMapMatch_Impl(content, matches, result);
  }

  // Round 3
  {
    std::map<std::string, std::string> matches = {
      {"Key1", "Match1"},
      {"{Key2", "Match2"},
      {"Key3", "Match3"},
    };
    std::string content = "xxxx{${Key1}}}yyy${{Key2}";
    std::string result("xxxx{Match1}}yyyMatch2");
    test_PrintMapMatch_Impl(content, matches, result);
  }

  std::cout << "Passed ^_^" << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: %s outputfile\n", argv[0]);
    return -1;
  }
  outputfile = argv[1];
  test_PrintVectorMatch();
  test_PrintMapMatch();
  return 0;

}