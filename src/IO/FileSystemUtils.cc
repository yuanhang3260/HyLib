#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#include "Base/Log.h"
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
  std::string result = "";
  for (auto& path_piece: args_list) {
    auto str = StringUtils::Strip(path_piece.as_string(), "/");
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
  if (pDir != NULL) {
    bExists = true;    
    (void)closedir(pDir);
  }
  return bExists;
}

bool CreateDir(StringPiece path, mode_t mode) {
  return mkdir(path.data(), mode) == 0;
}

bool CreateFile(StringPiece file_path) {
  FILE* pFile = fopen(file_path.data(), "w+");
  if (!pFile) {
    LogERROR("Can't open/create file %s", file_path.data());
    return false;
  }

  fclose(pFile);
  return true;
}

bool RemoveFile(StringPiece file_path) {
  if (!FileExists(file_path)) {
    return true;
  }
  return remove(file_path.data());
}

int ListDir(StringPiece path, std::vector<std::string>* result) {
  if (!result) {
    LogERROR("nullptr input to ListDir");
    return false;
  }

  if (!DirectoryExists(path)) {
    LogERROR("Directory %s doesn't exist", path.data());
    return -1;
  }

  result->clear();
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(path.data())) != nullptr) {
    // print all the files and directories within directory.
    while ((ent = readdir(dir)) != NULL) {
      result->emplace_back(ent->d_name);
    }
    closedir(dir);
    return result->size();
  }

  return -1;
}



}  // namespace FileSystem
