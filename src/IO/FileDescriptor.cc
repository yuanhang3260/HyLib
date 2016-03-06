#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#include "FileDescriptor.h"

namespace IO {

FileDescriptor::FileDescriptor(std::string filename, MODE mode) {
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
    fprintf(stderr, "ERROR: open file %s failed\n", filename.c_str());
    return;
  }
  closed_ = false;
}

int FileDescriptor::Read(void* buffer, const int nbytes) const {
  if (!closed_ && fd_ > 0) {
    return read(fd_, buffer, nbytes);
  }
  return -1;
}

int FileDescriptor::Write(const void* buf, const int nbytes) const {
  if (!closed_ && fd_ > 0) {
    return write(fd_, buf, nbytes);
  }
  return -1;
}

}