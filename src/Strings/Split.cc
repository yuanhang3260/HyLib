#include <functional>

#include "Base/BaseTypes.h"
#include "Base/Utils.h"
#include "Strings/Split.h"
#include "Utility/StringBuilder.h"

namespace Strings {

std::vector<std::string> Split(const std::string& str, char c,
                               Predicator predicator) {
  delimiters::SingleCharDelimiter delimiter(c);
  return Split(str, &delimiter, predicator);
}

std::vector<std::string> Split(const std::string& str,
                               const delimiters::AnyOf& char_set,
                               Predicator predicator) {
  return Split(str, &char_set, predicator);
}

std::vector<std::string> Split(const std::string& str,
                               const std::string& match,
                               Predicator predicator) {
  // Empty delimiter is a special case - It should return each character of the
  // string.
  if (match.empty()) {
    std::vector<std::string> result;
    for (uint32 i = 0; i < str.length(); i++) {
      result.push_back(std::string(1, str.at(i)));
    }
    return result;
  }

  delimiters::StringDelimiter delimiter(match);
  return Split(str, &delimiter, predicator);
}

std::vector<std::string> Split(const std::string& str,
                               const delimiters::Delimiter* delimiter,
                               Predicator predicator) {
  std::vector<std::string> result;

  unsigned int start = 0;
  for (uint32 i = 0; i < str.length(); i++) {
    if (delimiter->match(str, i)) {
      auto candidate = str.substr(start, i - start);
      if (!predicator || (predicator && !predicator(candidate))) {
        result.push_back(candidate);
      }
      start = i + delimiter->length();
    }
  }
  auto candidate = str.substr(start, str.length() - start);
  if (!predicator || (predicator && !predicator(candidate))) {
    result.push_back(candidate);
  }
  return result;
}

namespace  delimiters {

// SingleChar Delimiter.
SingleCharDelimiter::SingleCharDelimiter(char c) : c_(c) {}

bool SingleCharDelimiter::match(const std::string& str, int offset) const {
  return str.at(offset) == c_;
}

int SingleCharDelimiter::length() const {
  return 1;
}

// String Delimiter.
StringDelimiter::StringDelimiter(const std::string& str) : str_(str) {}

bool StringDelimiter::match(const std::string& str, int offset) const {
  if (str.length() - offset < str_.length()) {
    return false;
  }

  return str.substr(offset, str_.length()) == str_;
}

int StringDelimiter::length() const {
  return 1;
}

// AnyOf Delimiter.
AnyOf::AnyOf(const std::string& str) {
  for (uint32 i = 0; i < str.length(); i++) {
    candidates_.insert(str.at(i));
  }
}

int AnyOf::length() const {
  return 1;
}

bool AnyOf::match(const std::string& str, int offset) const {
  return candidates_.find(str.at(offset)) != candidates_.end();
}

bool AnyOf::has(char c) const {
  return candidates_.find(c) != candidates_.end();
}

void AnyOf::add(char c) {
  candidates_.insert(c);
}

void AnyOf::remove(char c) {
  auto it = candidates_.find(c);
  if (it != candidates_.end()) {
    candidates_.erase(it);
  }
}

}  // namespace delimiters

// Predicators.
Predicator default_predicator = std::function<bool(const std::string&)>();

namespace {

bool SkipWhiteSpace_Impl(const std::string& str) {
  for (uint32 i = 0; i < str.length(); i++) {
    if (str.at(i) != ' ' && str.at(i) != '\t') {
      return false;
    }
  }
  return true;
}

bool SkipEmpty_Impl(const std::string& str) {
  return str.empty();
}

bool RegExpPredicator_Impl(const std::string& regexp, const std::string& str) {
  // TODO: Implement this.
  return false;
}

}  // namepace

Predicator SkipWhiteSpace() {
  return SkipWhiteSpace_Impl;
}

Predicator SkipEmpty() {
  return SkipEmpty_Impl;
}

Predicator MakeRegExpPredicator(const std::string& regexp) {
  return std::bind(RegExpPredicator_Impl, regexp, std::placeholders::_1);
}

}  // namespce Strings
