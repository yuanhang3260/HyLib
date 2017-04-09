#ifndef BASE_PATH_H_
#define BASE_PATH_H_

#include <string>
#include <vector>
#include <initializer_list>

#include "Strings/StringPiece.h"

namespace Path {

// If the first path arg starts with '/' the result will also be an abstract
// path. Otherwise it returns a relative path.
std::string JoinPathImpl(std::initializer_list<StringPiece> arg_list);

template <typename... T>
std::string JoinPath(const T&... args) {
  return JoinPathImpl({args...});
}

}  // namespace FileSystem

#endif  /* BASE_PATH_H_ */