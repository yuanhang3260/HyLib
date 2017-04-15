#ifndef STRINGS_UTILS_H_
#define STRINGS_UTILS_H_

#include <functional>
#include <initializer_list>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>

#include "Strings/StringPiece.h"

namespace Strings {

bool IsCapitalLetter(const char c);
bool IsLowerCaseLetter(const char c);

std::string Upper(const std::string& str);
std::string Lower(const std::string& str);

bool IsLetterOrDigitOrUnderScore(const char c);

std::string Strip(const std::string& str);
std::string Strip(const std::string& str, const std::string& match);

bool StartWith(const std::string& str, const std::string& match);
bool EndWith(const std::string& str, const std::string& match);

bool IsSingleWord(const std::string str);

std::string StrCatImpl(std::initializer_list<StringPiece> arg_list);

template <typename... T>
std::string StrCat(const T&... args) {
  return StrCatImpl({args...});
}

std::string Join(const std::vector<std::string>& list,
                 const std::string& seperator);

std::string IntToHexString(int i);

int FindFirstMatch(const std::string& str, const std::string& match);
int FindFirstMatch(const std::string& str, const std::string& match,
                   int offset);
int FindLastMatch(const std::string& str, const std::string& match);

std::string ReplaceWith(const std::string& str,
                        const std::string& old,
                        const std::string& match);

// extract tokens from a string.
std::vector<std::string> ExtractTokens(std::string* str, char start, char end);

} // namespace Strings

#endif /* STRINGS_UTILS_H_ */
