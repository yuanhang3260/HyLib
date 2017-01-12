#ifndef STRING_BUILDR_
#define STRING_BUILDR_

#include <string>

#include "Base/BaseTypes.h"

namespace Utility {

class StringBuilder {
 public:
  StringBuilder();
  explicit StringBuilder(int size);
  ~StringBuilder();

  const char* CharArray() const;
  unsigned int size() const;
  int Length() const { return size(); }
  int Empty() const { return size() == 0; }
  char At(int32 pos) const;
  std::string ToString() const;
  std::string SubStr(int start, int size) const;
  std::string Prefix(int size) const;
  std::string Suffix(int size) const;
  bool StartWith(const std::string str);
  bool EndWith(const std::string str);

  void Clear();
  void Truncate(const int size);
  void Append(const char c);
  void Append(const char* str, const int size);
  void Append(std::string str);

 private:
  static const int DEFAULT_SIZE = 128;
  int buf_size_ = 0;
  int extend_size_ = 128;
  int size_ = 0;
  char* buf_ = nullptr;
};

}

#endif /* STRING_BUILDR_ */