#ifndef FILE_SYSTEM_UTILS_
#define FILE_SYSTEM_UTILS_

#include <string>
#include <initializer_list>

#include "Base/StringPiece.h"

namespace FileSystem {

std::string GetAbstractPath(StringPiece filename);


std::string JoinPathImpl(std::initializer_list<StringPiece> arg_list);

template <typename... T>
std::string JoinPath(const T&... args) {
  return JoinPathImpl({args...});
}
 


}  // namespace FileSystem

#endif  /* FILE_SYSTEM_UTILS_ */