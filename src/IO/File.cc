#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "Base/Log.h"
#include "File.h"

namespace File {

int FileSize(StringPiece filename) {
  struct stat stat_buf;
  int rc = stat(filename.data(), &stat_buf);
  if (rc != 0) {
    LogERROR("Stats file %s faild", filename.data());
    return -1;
  }
  return stat_buf.st_size;
}

bool GetContent(StringPiece file_path, std::string* content) {
  int file_size = FileSize(file_path);
  if (file_size < 0) {
    LogERROR("Invalid file %s, file size = %d", file_path.data(), file_size);
    return false;
  }

  FILE * pFile;
  pFile = fopen(file_path.data(), "r");
  if (!pFile) {
    LogERROR("Can't open file %s", file_path.data());
    return false;
  }

  char buffer[file_size];
  int readlen = fread(buffer, 1, file_size, pFile);
  if (readlen != file_size) {
    LogERROR("Read file %s error", file_path.data());
    fclose(pFile);
    return false;
  }

  content->assign(buffer, file_size);
  fclose(pFile);
  return true;
}

bool SetContent(StringPiece file_path, StringPiece content) {
  FILE* pFile = fopen(file_path.data(), "w+");
  if (!pFile) {
    LogERROR("Can't open file %s", file_path.data());
    return false;
  }

  int writelen = fwrite(content.data(), 1, content.length(), pFile);
  if (writelen != content.length()) {
    LogERROR("Set content for file %s failed", file_path.data());
    fclose(pFile);
    return false;
  }

  fclose(pFile);
  return true;
}

}  // namespace File
