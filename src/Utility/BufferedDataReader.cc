#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>

#include "Base/Log.h"
#include "BufferedDataReader.h"
#include "Utility/StringBuilder.h"

namespace Utility {

// Constructors
BufferedDataReader::BufferedDataReader(
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

BufferedDataReader::BufferedDataReader(IO::FileDescriptorInterface* fd) {
  fdscrpt_ = fd;
  bufSize = BUFSIZE;
  buffer = new char[bufSize];
}

// read a byte
int BufferedDataReader::Read(char* c) {
  int re;
  if (dataLen == 0 && (re = refill()) <= 0) {
     return re;
  }

  *c = buffer[tail++];
  dataLen--;
  return 1;
}

// read a chunk of data from buffer
int BufferedDataReader::Read(char* buf, int off, const int len) {    
  if (!checkArgs(buf, off, len)) {
    return -1;
  }

  if (len == 0) {
    return 0;
  }
  
  int readnLeft = len;
  while (readnLeft > 0) {
    if (dataLen <= 0) {
      // Directly copy data to user buffer.
      if (readnLeft >= bufSize) {
        int re = fdscrpt_->Read(buf + off, readnLeft);
        readnLeft -= re;
        break;
      }
      else {
        // Re-fill the internal buffer.
        int nread;
        if ((nread = refill()) <= 0) {
          if (len == readnLeft) {
            // Nothing has been read at all. Return 0 or error code.
            return nread;
          }
          return len - readnLeft;
        }
      }
    }
    int copyLen = dataLen < readnLeft? dataLen : readnLeft;
    memcpy(buf + off, buffer + tail, copyLen);
    off += copyLen;
    tail += copyLen;
    dataLen -= copyLen;
    readnLeft -= copyLen;
    // Read length reaches expected length
    if (readnLeft == 0) {
      break;
    }
    // Nothing is available in pipe.
    // if (fd != null && fd.available() <= 0) {
    //   break;
    // }
  }
  return len - readnLeft;
}

// Read one line from buffer.
int BufferedDataReader::ReadLine(std::string* str,
                                 const std::string& line_breaker) {
  // Reach file end.
  int re = 0;

  bool got_line = false, eof = false;
  Utility::StringBuilder str_builder;
  while (true) {
    if (dataLen == 0 && (re = refill()) <= 0) {
      //LogINFO("LOG: Refill in readline() ended.\n");
      if (re == 0) {
        // End of file (socket closed).
        readline_record_.clear();
        LogINFO("EOF");
        eof = true;
        break;
      } else {
        // Errors:
        // For non-blocking sockets, returning EGAIN or EWOULDBLOCK on empty
        // receiving buffer is actually not an error. We need to store
        // already read bytes into readline_record_ for future read.
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          if (!str_builder.Empty()) {
            readline_record_ += str_builder.ToString();
          }
        }
        LogERROR("#### ERROR Read %d", errno);
        return -1;
      }
    }

    str_builder.Append(buffer[tail++]);
    dataLen--;
    if (str_builder.EndWith(line_breaker)) {
      str_builder.Truncate(str_builder.Length() - line_breaker.length());
      got_line = true;
      break;
    }
  }

  *str = readline_record_ + str_builder.ToString();
  readline_record_.clear();
  if (got_line) {
    return str->length();
  } else if (eof && str->length() > 0) {
    return str->length();
  }

  return -1;
}

// Re-fill the internal buffer
int BufferedDataReader::refill() {
  head = 0;
  tail = 0;
  dataLen = 0;
  int readn = fdscrpt_->Read(buffer, bufSize);
  // printf("buffer read %d bytes\n", readn);
  // for (int i = 0; i < readn; i++) {
  //   printf("%c", buffer[i]);
  // }
  // printf("]\n");
  if (readn <= 0) {
    dataLen = 0;
  }
  else {
    head += readn;
    dataLen = readn;
  }
  return readn;
}

// Close the pipe
int BufferedDataReader::Close() {
  delete[] buffer;
  head = tail = 0;
  dataLen = 0;
  return 0;
}

void BufferedDataReader::Reset() {
  delete[] buffer;
  buffer = new char[bufSize];
  head = tail = 0;
  dataLen = 0;
}

// Check user arguments.
bool BufferedDataReader::checkArgs(char* buf, const int off, const int len) {
  if (buf == NULL) {
    return false;
  }
  
  if (len < 0 || off < 0) {
    return false;
  }
  return true;
}

// Compare contents of two buffers.
bool BufferedDataReader::dataCompare(const char* buf1,
                                     const int off1,
                                     const char* buf2,
                                     const int off2,
                                     const int length) {
  for (int i = 0; i < length; i++) {
    if (buf1[off1+i] != buf2[off2+i]) {
      return false;
    }
  }
  return true;
}

}  /// namespace Utility
