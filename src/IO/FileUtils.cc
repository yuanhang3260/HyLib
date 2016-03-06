#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "FileUtils.h"

namespace IO {

std::string FileUtils::GetAbstractPath(std::string filename) {
  if (access(filename.c_str(), F_OK) < 0) {
    return "";
  }
  char fullpath[PATH_MAX];
  char* ptr = realpath(filename.c_str(), fullpath);
  return std::string(ptr);
}

}  // namespace IO