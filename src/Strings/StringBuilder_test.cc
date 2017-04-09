#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "StringBuilder.h"

int main(int argc, char** argv) {
  {
    Strings::StringBuilder strBuilder;
    strBuilder.Append('a');
    if (strBuilder.ToString() != "a") {
      fprintf(stderr, "ERROR: Expect \"a\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Append('b');
    if (strBuilder.ToString() != "ab") {
      fprintf(stderr, "ERROR: Expect \"ab\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Append("cdefg", 3);
    if (strBuilder.ToString() != "abcde") {
      fprintf(stderr, "ERROR: Expect \"abcde\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Append("hi");
    if (strBuilder.ToString() != "abcdehi") {
      fprintf(stderr, "ERROR: Expect \"abcdehi\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Truncate(6);
    if (strBuilder.ToString() != "abcdeh") {
      fprintf(stderr, "ERROR: Expect \"abcdehi\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Truncate(7);
    if (strBuilder.ToString() != "abcdeh") {
      fprintf(stderr, "ERROR: Expect \"abcdeh\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }

    if (strBuilder.SubStr(2, 4) != "cdeh") {
      fprintf(stderr, "ERROR: Expect \"cdeh\", Actual \"%s\"\n",
                      strBuilder.SubStr(2, 4).c_str());
      exit(-1);
    }

    if (strBuilder.SubStr(-1, 4) != "") {
      fprintf(stderr, "ERROR: Expect \"\", Actual \"%s\"\n",
                      strBuilder.SubStr(-1, 4).c_str());
      exit(-1);
    }

    if (strBuilder.SubStr(3, 4) != "") {
      fprintf(stderr, "ERROR: Expect \"\", Actual \"%s\"\n",
                      strBuilder.SubStr(3, 4).c_str());
      exit(-1);
    }

    strBuilder.Clear();
    if (strBuilder.ToString() != "") {
      fprintf(stderr,"ERROR: Expect \"\", Actual \"%s\"\n",
                      strBuilder.ToString().c_str());
      exit(-1);
    }
  }

  {
    Strings::StringBuilder strBuilder(3);
    strBuilder.Append("aaa");
    if (strBuilder.ToString() != "aaa" || strBuilder.size() != 3) {
      fprintf(stderr,
              "ERROR: Expect \"aaa\", Actual \"%s\"\n",
              strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Append('A');
    if (strBuilder.ToString() != "aaaA" || strBuilder.size() != 4) {
      fprintf(stderr,
              "ERROR: Expect \"aaaA\", Actual \"%s\"\n",
              strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Append("bcdefg");
    if (strBuilder.ToString() != "aaaAbcdefg" || strBuilder.size() != 10) {
      fprintf(stderr,
              "ERROR: Expect \"abcdefg\", Actual \"%s\"\n",
              strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Append("xyz!@#$%^&,.");
    if (strBuilder.ToString() != "aaaAbcdefgxyz!@#$%^&,." ||
        strBuilder.size() != 22) {
      fprintf(stderr,
              "ERROR: Expect \"aaaAbcdefgxyz!@#$%%^&,.\", Actual \"%s\"\n",
              strBuilder.ToString().c_str());
      exit(-1);
    }
    strBuilder.Clear();
    if (strBuilder.size() != 0) {
      fprintf(stderr, "ERROR: Expect size = 0, Actual %d\n", strBuilder.size());
      exit(-1);
    }
  }

  printf("\033[2;32mPassed ^_^\033[0m\n");
}