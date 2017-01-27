#ifndef STRINGS_SPLIT_H_
#define STRINGS_SPLIT_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>

namespace Strings {

namespace  delimiters {

// Delimiters.
class Delimiter {
 public:
  virtual bool match(const std::string& str, int offset) const = 0;
  virtual int length() const = 0;
};

class SingleCharDelimiter : public Delimiter {
 public:
  SingleCharDelimiter(char c);

  bool match(const std::string& str, int offset) const override;
  int length() const override;

 private:
  char c_;
};

class StringDelimiter : public Delimiter {
 public:
  StringDelimiter(const std::string& str);

  bool match(const std::string& str, int offset) const override;
  int length() const override;

 private:
  std::string str_;
};

class AnyOf : public Delimiter {
 public:
  AnyOf(const std::string& str);
  bool has(char c) const;
  void add(char c);
  void remove(char c);

  bool match(const std::string& str, int offset) const override;
  int length() const override;

 private:
  std::set<char> candidates_;
};

}  // delimiters

// Predicators.
using Predicator = std::function<bool(const std::string&)>;
extern Predicator default_predicator;

Predicator SkipWhiteSpace();
Predicator SkipEmpty();
Predicator MakeRegExpPredicator(const std::string& regexp);

// Split.
std::vector<std::string> Split(const std::string& str, char c,
                               Predicator predicator=default_predicator);
std::vector<std::string> Split(const std::string& str,
                               const delimiters::AnyOf& char_set,
                               Predicator predicator=default_predicator);
std::vector<std::string> Split(const std::string& str, const std::string& match,
                               Predicator predicator=default_predicator);

std::vector<std::string> Split(const std::string& str,
                               const delimiters::Delimiter* delimiter,
                               Predicator predicator);

} // namespace Strings

#endif /* STRINGS_SPLIT_H_ */
