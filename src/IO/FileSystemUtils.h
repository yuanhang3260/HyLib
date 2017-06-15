#ifndef FILE_SYSTEM_UTILS_
#define FILE_SYSTEM_UTILS_

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <initializer_list>

#include "Base/BaseTypes.h"
#include "Strings/StringPiece.h"

namespace FileSystem {

// Get abstract path of a file. The file must exist, otherwise it returns "".
std::string GetAbstractPath(StringPiece filename);

// If the first path arg starts with '/' the result will also be an abstract
// path. Otherwise it returns a relative path.
std::string JoinPathImpl(std::initializer_list<StringPiece> arg_list);

template <typename... T>
std::string JoinPath(const T&... args) {
  return JoinPathImpl({args...});
}

// FileSystem utilities.
bool DirectoryExists(StringPiece dir_path);
bool CreateDir(StringPiece path);
bool CreateDirRecursive(StringPiece path);
bool CreateDir(StringPiece path, mode_t mode);
int ListDir(StringPiece path, std::vector<std::string>* result);

bool FileExists(StringPiece file_path);
bool CreateFile(StringPiece file_path);
int64 FileSize(StringPiece path);
bool TruncateFile(StringPiece path, uint32 size);
bool RenameFile(StringPiece path, StringPiece new_filename);
std::string ParentDir(StringPiece path);

// Remove file/dir.
bool Remove(StringPiece file_path); 


}  // namespace FileSystem

#endif  /* FILE_SYSTEM_UTILS_ */