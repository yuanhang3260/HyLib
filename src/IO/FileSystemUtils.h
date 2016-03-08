#ifndef FILE_SYSTEM_UTILS_
#define FILE_SYSTEM_UTILS_

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <initializer_list>

#include "Base/StringPiece.h"

namespace FileSystem {

// Get abstract path of a file. The file must exist, otherwise it returns "".
std::string GetAbstractPath(StringPiece filename);

// Join path piece into an abstract path.
std::string JoinPathImpl(std::initializer_list<StringPiece> arg_list);

template <typename... T>
std::string JoinPath(const T&... args) {
  return JoinPathImpl({args...});
}

// FileSystem utilities.
bool FileExists(StringPiece file_path);
bool DirectoryExists(StringPiece dir_path);
bool CreateDir(StringPiece path, mode_t mode);



}  // namespace FileSystem

#endif  /* FILE_SYSTEM_UTILS_ */