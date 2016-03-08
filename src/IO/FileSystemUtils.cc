#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#include "Utility/Strings.h"
#include "FileSystemUtils.h"

namespace FileSystem {

std::string GetAbstractPath(StringPiece filename) {
  if (access(filename.data(), F_OK) < 0) {
    return "";
  }
  char fullpath[PATH_MAX];
  char* ptr = realpath(filename.data(), fullpath);
  return std::string(ptr);
}

std::string JoinPathImpl(
                std::initializer_list<StringPiece> args_list) {
  if (args_list.size() == 0) {
    return "";
  }

  std::string result = "";
  auto it = args_list.begin();
  if ((*it)[0] == '/') {
    result = "/";
  }
  result += StringUtils::Strip(it->as_string(), "/");
  it++;

  for (; it != args_list.end(); it++) {
    auto str = StringUtils::Strip(it->as_string(), "/");
    result += ("/" + str);
  }
  return result;
}

bool FileExists(StringPiece file_path) {
  return (access(file_path.data(), F_OK) != -1);
}

bool DirectoryExists(StringPiece path) {
  DIR *pDir;
  bool bExists = false;
  pDir = opendir(path.data());
  if (pDir != nullptr) {
    bExists = true;    
    (void)closedir(pDir);
  }
  return bExists;
}

bool CreateDir(StringPiece path, mode_t mode) {
  return mkdir(path.data(), mode);
}

}  // namespace FileSystem
