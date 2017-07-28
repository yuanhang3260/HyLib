#include <sstream>

#include "Base/BaseTypes.h"
#include "Base/Utils.h"
#include "Strings/StringBuilder.h"
#include "Strings/Utils.h"

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

std::string GetPrefix(const std::string& str, uint32 length) {
  if (length >= str.length()) {
    return str;
  }

  return str.substr(0, length);
}

std::string GetSuffix(const std::string& str, uint32 length) {
  if (length >= str.length()) {
    return str;
  }

  return str.substr(str.length()- length);
}

std::string RemovePrefix(const std::string& str, uint32 length) {
  if (length >= str.length()) {
    return "";
  }

  return str.substr(length);
}

std::string RemoveSuffix(const std::string& str, uint32 length) {
  if (length >= str.length()) {
    return "";
  }

  return str.substr(0, str.length()- length);
}

uint32 LongestCommonPrefix(const std::string& str1, const std::string& str2) {
  uint32 len = std::min(str1.length(), str2.length());
  uint32 i = 0;
  for (; i < len; i++) {
    if (str1.at(i) != str2.at(i)) {
      return i;
    }
  }
  return i;
}

uint32 LongestCommonSuffix(const std::string& str1, const std::string& str2) {
  uint32 len = std::min(str1.length(), str2.length());
  uint32 i = 0;
  uint32 str1_pos = str1.length() - 1;
  uint32 str2_pos = str2.length() - 1;
  for (; i < len; i++) {
    if (str1.at(str1_pos - i) != str2.at(str2_pos - i)) {
      return i;
    }
  }
  return i;
}

bool IsSingleWord(const std::string& str) {
  if (str.empty()) {
    return false;
  }
  return str.find(' ') == std::string::npos;
}

std::string StrCatImpl(std::initializer_list<StringPiece> arg_list) {
  StringBuilder str_builder;
  for (const auto& ele : arg_list) {
    str_builder.Append(ele);
  }
  return str_builder.ToString();
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
  Strings::StringBuilder str_builder;
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
  StringBuilder str_builder;
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

std::string Join(const std::vector<std::string>& list,
                 const std::string& seperator) {
  if (list.empty()) {
    return "";
  }

  StringBuilder str_builder;
  uint32 size = list.size();
  for (uint32 i = 0; i < size - 1; i++) {
    str_builder.Append(list.at(i));
    str_builder.Append(seperator);
  }
  str_builder.Append(list.at(size - 1));
  return str_builder.ToString();
}

}  // namespce Strings
