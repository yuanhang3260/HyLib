#ifndef __BUFFERED_DATA_READER__
#define __BUFFERED_DATA_READER__

#include <iostream>
#include <string>
#include <memory>

#include "IO/FileDescriptorInterface.h"

namespace Utility {

class BufferedDataReader {
 public:
  // Constructors
  BufferedDataReader(IO::FileDescriptorInterface* fd, int bufSize);

  BufferedDataReader(IO::FileDescriptorInterface* fd);

  virtual ~BufferedDataReader() {
    //std::cout << "deleting BufferedDataReader\n";
    Close();
  }

  // read a byte
  int Read(char* c);

  // read a chunk of data from buffer
  int Read(char* buf, int off, const int len);

  enum ReadLineResult {
    NEW_LINE,
    PIPE_CLOSED,
    WAIT_FOR_PIPE,
    ERROR,
  };

  // Read one line from buffer. Return length of read line or -1 on failure.
  ReadLineResult ReadLine(std::string* str,
                          const std::string& line_breaker="\r\n");

  // Close the pipe
  int Close();

  // Reset
  void Reset();

  // Compare contents of two buffers.
  static bool dataCompare(const char* buf1,
                          const int off1,
                          const char* buf2,
                          const int off2,
                          const int len);

private:
  static const int BUFSIZE = 1024;
  static const int MAX_BUFSIZE = 4194302;

  int bufSize;
  char* buffer;
  IO::FileDescriptorInterface* fdscrpt_;
  int head = 0;
  int tail = 0;
  int dataLen = 0; // effective data lengt
  // Re-fill the internal buffer
  int refill();

  std::string readline_buffer_;

  // Check user arguments.
  bool checkArgs(char* buf, const int off, const int len);
};

}  // namespace Utility

#endif /* __BUFFERED_DATA_READER__ */