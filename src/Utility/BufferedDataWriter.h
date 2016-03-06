#ifndef __BUFFERED_DATA_WRITER__
#define __BUFFERED_DATA_WRITER__

#include <iostream>
#include <string>
#include <memory>

#include "../IO/FileDescriptorInterface.h"

namespace Utility {

class BufferedDataWriter {
 public:
  // Constructors
  BufferedDataWriter(
      IO::FileDescriptorInterface* fd, int bufSize);

  BufferedDataWriter(IO::FileDescriptorInterface* fd);

  virtual ~BufferedDataWriter() {
    //std::cout << "deleting BufferedDataWriter\n";
    Close();
  }

  // write a byte
  int Write(const char c);

  // write a chunk of data from buffer
  int Write(const char* buf, int off, const int len);

  // flush the buffer
  int Flush();

  // Close the pipe
  int Close();

  // Reset
  void Reset();

private:
  static const int BUFSIZE = 1024;
  static const int MAX_BUFSIZE = 4194302;

  int bufSize;
  char* buffer;
  IO::FileDescriptorInterface* fdscrpt_;
  int head = 0;
  int tail = 0;
  int dataLen = 0;

  // Check user arguments.
  bool checkArgs(char* buf, const int off, const int len);
};

}  // namespace Utility

#endif /* __BUFFERED_DATA_WRITER__ */