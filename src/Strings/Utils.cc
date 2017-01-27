#include <sstream>

#include "Base/BaseTypes.h"
#include "Base/Utils.h"
#include "Strings/Utils.h"
#include "Utility/StringBuilder.h"

namespace Strings {

bool IsCapitalLetter(const char c) {
  return c >= 'A' && c <= 'Z';
}

bool IsLowerCaseLetter(const char c) {
  return c >= 'a' && c <= 'z';
}

std::string Upper(const std::string& str) {
  char c[str.length()];
  for (unsigned int i = 0; i < str.length(); i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      c[i] = str[i] - 32;
    }
    else {
      c[i] = str[i];
    }
  }
  return std::string(c, str.length());
}

std::string Lower(const std::string& str) {
  char c[str.length()];
  for (unsigned int i = 0; i < str.length(); i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      c[i] = str[i] + 32;
    }
    else {
      c[i] = str[i];
    }
  }
  return std::string(c, str.length());
}

bool IsLetterOrDigitOrUnderScore(const char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9') ||
         (c == '_');
}

std::string Strip(const std::string& str) {
  if (str.length() <= 1) {
    return str;
  }
  unsigned int i, j;
  for (i = 0; i < str.length(); i++) {
    if (str[i] != ' ' && str[i] != '\t') {
      break;
    }
  }
  for (j = str.length() - 1; j >= 0; j--) {
    if (str[j] != ' ' && str[j] != '\t') {
      break;
    }
  }
  return str.substr(i, j + 1 - i);
}

std::string Strip(const std::string& str, const std::string& match) {
  std::string str1 = Strip(str);
  if (match.length() == 0) {
    return str1;
  }
  // strip head.
  int limit = Utils::Min(str.length(), match.length());
  int i = 0;
  for (; i < limit; i++) {
    if (str[i] != match[i]) {
      break;
    }
  }

  int j = str.length() - 1, k = match.length() - 1;
  for (; j >= 0 && k >= 0; j--, k--) {
    if (str[j] != match[k]) {
      break;
    }
  }
  // printf("i = %d, j = %d\n", i, j);
  return str.substr(i, j + 1 - i);
}

bool StartWith(const std::string& str, const std::string& match) {
  if (match.length() > str.length()) {
    return false;
  }
  for (unsigned int i = 0; i < match.length(); i++) {
    if (str[i] != match[i]) {
      return false;
    }
  }
  return true;
}

bool EndWith(const std::string& str, const std::string& match) {
  if (match.length() > str.length()) {
    return false;
  }
  int strlength = str.length(), matchlength = match.length();
  for (unsigned int i = 0; i < match.length(); i++) {
    if (str[strlength - 1 - i] != match[matchlength - 1 - i]) {
      return false;
    }
  }
  return true;
}

bool IsSingleWord(const std::string& str) {
  if (str.empty()) {
    return false;
  }
  return str.find(' ') == std::string::npos;
}

std::string StrCat(std::vector<std::string> v, unsigned int start = 0) {
  std::string result = "";
  for (unsigned int i = start; i < v.size(); i++) {
    result += v[i];
  }
  return result;
}

std::string IntToHexString(int i) {
  std::stringstream stream;
  stream << "0x" 
         << std::hex << i;
  return stream.str();
}

int FindFirstMatch(const std::string& str, const std::string& match) {
  if (match.length() > str.length()) {
    return -1;
  }
  for (unsigned int i = 0; i <= str.length() - match.length(); i++) {
    if (str.substr(i, match.length()) == match) {
      return i;
    }
  }
  return -1;
}

int FindFirstMatch(
    const std::string& str, const std::string& match, int offset) {
  if (match.length() - offset > str.length()) {
    return -1;
  }
  for (unsigned int i = offset; i <= str.length() - match.length(); i++) {
    if (str.substr(i, match.length()) == match) {
      return i;
    }
  }
  return -1;
}

int FindLastMatch(const std::string& str, const std::string& match) {
  if (match.length() > str.length()) {
    return -1;
  }
  for (unsigned int i = str.length() - match.length(); i >= 0; i--) {
    if (str.substr(i, match.length()) == match) {
      return i;
    }
  }
  return -1;
}

std::string ReplaceWith(const std::string& str,
                        const std::string& old,
                        const std::string& rep) {
  Utility::StringBuilder str_builder;
  for (unsigned int i = 0; i <= str.length() - old.length(); i++) {
    if (str.substr(i, old.length()) == old) {
      str_builder.Append(rep);
    }
    else {
      str_builder.Append(str[i]);
    }
  }
  return str_builder.ToString();
}

std::vector<std::string> ExtractTokens(std::string* str, char start, char end) {
  Utility::StringBuilder str_builder;
  std::vector<std::string> result;
  bool matching = false;
  int matching_start = 0;
  for (unsigned int i = 0; i < str->length(); i++) {
    if (!matching) {
      if ((*str)[i] == start) {
        matching = true;
        matching_start = i;
      }
      else {
        str_builder.Append((*str)[i]);
      }
    }
    else {
      if ((*str)[i] == end) {
        result.push_back(str->substr(matching_start, i + 1 - matching_start));
        matching = false;
      }
    }
  }
  *str = str_builder.ToString();
  return result;
}

}  // namespce Strings
