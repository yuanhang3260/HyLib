#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "Utility/Strings.h"
#include "FileSystemUtils.h"

namespace FileSystem {

std::string GetAbstractPath(std::string filename) {
  if (access(filename.c_str(), F_OK) < 0) {
    return "";
  }
  char fullpath[PATH_MAX];
  char* ptr = realpath(filename.c_str(), fullpath);
  return std::string(ptr);
}

std::string JoinPathImpl(
                std::initializer_list<StringPiece> args_list) {
  std::string result = "";
  for (auto& path_piece: args_list) {
    auto str = StringUtils::Strip(path_piece.as_string(), "/");
    result += ("/" + str);
  }
  return result;
}

}  // namespace FileSystem
