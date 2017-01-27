#ifndef STRINGS_UTILS_H_
#define STRINGS_UTILS_H_

#include <functional>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>

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

std::string StrCat(std::vector<std::string>, unsigned int start);

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
