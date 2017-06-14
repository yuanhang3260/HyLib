#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#include "Base/Log.h"
#include "IO/FileDescriptor.h"

namespace IO {

FileDescriptor::FileDescriptor(int fd) {
  if (fd > 0) {
    fd_ = fd;
    closed_ = false;
  }
}

FileDescriptor::FileDescriptor(const std::string& filename, MODE mode) {
  switch (mode) {
    case READ_ONLY:
      fd_ = open(filename.c_str(), O_RDONLY);
      break;
    case WRITE_ONLY:
      fd_ = open(filename.c_str(),
                 O_WRONLY | O_TRUNC | O_CREAT,
                 S_IRUSR | S_IWUSR);
      break;
    case READ_WRITE:
      fd_ = open(filename.c_str(), O_RDWR);
      break;
    default:
      break;
  }
  if (fd_ < 0) {
    LogERROR("ERROR: open file %s failed\n", filename.c_str());
    return;
  }
  closed_ = false;
}

int FileDescriptor::Close() {
  if (!closed_ && fd_ > 0) {
    int re = close(fd_);
    if (re != 0) {
      return re; 
    }
    closed_ = true;
    return 0;
  }
  return -1;
}

int FileDescriptor::Read(void* buffer, int nbytes) {
  if (!closed_ && fd_ > 0) {
    return read(fd_, buffer, nbytes);
  }
  return -1;
}

int FileDescriptor::Write(const void* buf, int nbytes) const {
  if (!closed_ && fd_ > 0) {
    return write(fd_, buf, nbytes);
  }
  return -1;
}

}