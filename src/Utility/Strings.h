#ifndef UTILITY_STRINGS_H_
#define UTILITY_STRINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace StringUtils {

bool IsCapitalLetter(const char c);
bool IsLowerCaseLetter(const char c);

std::string Upper(std::string str);
std::string Lower(std::string str);

bool IsLetterOrDigitOrUnderScore(const char c);

std::string Strip(std::string str);
std::string Strip(std::string str, std::string match);

bool StartWith(std::string str, std::string match);
bool EndWith(std::string str, std::string match);

std::vector<std::string> Split(const std::string str, const char c);
std::vector<std::string> Split(const std::string str, const std::string match);

std::vector<std::string> SplitGreedy(const std::string str, const char c);
std::vector<std::string> SplitGreedy(const std::string str,
                                     const std::string match);

bool IsSingleWord(const std::string str);

std::string StrCat(std::vector<std::string>, unsigned int start);

std::string IntToHexString(int i);

int FindFirstMatch(std::string str, std::string match);
int FindFirstMatch(std::string str, std::string match, int offset);
int FindLastMatch(std::string str, std::string match);

std::string ReplaceWith(std::string str, const char old, const char match);
std::string ReplaceWith(std::string str,
                        const std::string old,
                        const std::string match);

// extract tokens from a string.
std::vector<std::string> ExtractTokens(std::string* str, char start, char end);

} // namespace StringUtils

#endif /* UTILITY_STRINGS_H_ */
