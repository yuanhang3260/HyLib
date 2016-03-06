#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>

#include "BufferedDataWriter.h"

namespace Utility {

BufferedDataWriter::BufferedDataWriter(
    IO::FileDescriptorInterface* fd, int bufSize) {
  fdscrpt_ = fd;

  if (bufSize < 0 || bufSize > MAX_BUFSIZE) {
    bufSize = 1024;
  }
  else {
    this->bufSize = bufSize;
  }
  buffer = new char[bufSize];
}

BufferedDataWriter::BufferedDataWriter(IO::FileDescriptorInterface* fd) {
  fdscrpt_ = fd;
  bufSize = BUFSIZE;
  buffer = new char[bufSize];
}

int BufferedDataWriter::Write(const char c) {
  if (dataLen >= bufSize) {
    int nflush = Flush();
    if (nflush <= 0) {
      return nflush;
    }
  }
  buffer[tail++] = c;
  dataLen++;
  return 1;
}

int BufferedDataWriter::Write(const char* src_buf, int off, const int len) {
  // check args

  int left_to_write = len;
  while (left_to_write > 0) {
    if (dataLen >= bufSize) {
      int nflush = Flush();
      if (nflush <= 0) {
        return len - left_to_write;
      }
      if (dataLen == 0 && left_to_write >= bufSize) {
        int nwrite = fdscrpt_->Write(src_buf + off, left_to_write);
        left_to_write -= nwrite;
        break;
      }
    }

    int copy_len = left_to_write < (bufSize - dataLen) ? left_to_write :
                                                         bufSize - dataLen;
    memcpy(buffer + tail, src_buf + off, copy_len);
    tail += copy_len;
    dataLen += copy_len;
    off += copy_len;
    left_to_write -= copy_len;
  }
  return len;
}

int BufferedDataWriter::Flush() {
  if (dataLen <= 0) {
    return 0;
  }
  // printf("flushing %d bytes\n", dataLen);
  int nwrite = fdscrpt_->Write(buffer, dataLen);
  if (nwrite <= 0) {
    return nwrite;
  }

  head += nwrite;
  dataLen -= nwrite;
  if (head < tail) {
    memmove(buffer, buffer + head, dataLen);
  }
  head = 0;
  tail = dataLen;
  return nwrite;
}

int BufferedDataWriter::Close() {
  Flush();
  delete[] buffer;
  head = tail = 0;
  dataLen = 0;
  return 0;
}

void BufferedDataWriter::Reset() {
  delete[] buffer;
  buffer = new char[bufSize];
  head = tail = 0;
  dataLen = 0;
}

}  // namespace Utility