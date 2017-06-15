#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#include "Base/Log.h"
#include "Base/Path.h"
#include "Strings/Split.h"
#include "Strings/Utils.h"
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
  result += Strings::Strip(it->as_string(), "/");
  it++;

  for (; it != args_list.end(); it++) {
    auto str = Strings::Strip(it->as_string(), "/");
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

bool CreateDir(StringPiece path) {
  return CreateDir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

bool CreateDir(StringPiece path, mode_t mode) {
  if (DirectoryExists(path)) {
    return true;
  }
  return mkdir(path.data(), mode) == 0;
}

bool CreateDirRecursive(StringPiece path) {
  auto dirs = Strings::Split(path.ToString(), "/", Strings::SkipWhiteSpace());
  std::string crt_dir_path;
  if (path.starts_with("/")) {
    crt_dir_path = "/";
  }

  for (const auto& dir : dirs) {
    crt_dir_path = Strings::StrCat(crt_dir_path, dir, "/");
    if (!CreateDir(crt_dir_path)) {
      return false;
    }
  }
  return true;
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

bool Remove(StringPiece file_path) {
  if (!FileExists(file_path)) {
    return true;
  }
  return remove(file_path.data()) == 0;
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

int64 FileSize(StringPiece path) {
  struct stat stat_buf;
  int re = stat(path.data(), &stat_buf);
  if (re < 0) {
    LogERROR("Failed to stat file %s", path.data());
    return 0;
  }

  return stat_buf.st_size;
}

bool TruncateFile(StringPiece path, uint32 size) {
  return truncate(path.data(), size) == 0;
}

std::string ParentDir(StringPiece path) {
  if (path == "/") {
    return "";
  }

  if (!FileExists(path)) {
    return "";
  }
  std::string abs_path = GetAbstractPath(path.ToString());
  if (abs_path.empty()) {
    return "";
  }

  // Is it possible?
  if (Strings::EndWith(abs_path, "/")) {
    abs_path = Strings::RemoveSuffix(abs_path, 1);
  }

  int last_slash = Strings::FindLastMatch(abs_path, "/");
  std::string parent_dir = Strings::GetPrefix(abs_path, last_slash);
  if (parent_dir.empty()) {
    // File is in root dir, parent is still "/"
    return "/";
  }

  return parent_dir;
}

bool RenameFile(StringPiece path, StringPiece new_filename) {
  if (!FileExists(path)) {
    LogERROR("File \"%s\" doesn't exist", path.data());
    return false;
  }

  std::string parent_dir = ParentDir(path);
  if (parent_dir.empty()) {
    return false;
  }

  return rename(path.data(),
                Path::JoinPath(parent_dir, new_filename).c_str()) == 0;
}


}  // namespace FileSystem
