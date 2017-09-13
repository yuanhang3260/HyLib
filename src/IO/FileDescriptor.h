#ifndef FILE_DESCRIPTOR_H_
#define FILE_DESCRIPTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

#include "Base/BaseTypes.h"
#include "IO/FileDescriptorInterface.h"

namespace IO {

class FileDescriptor : public FileDescriptorInterface {
 public:
  enum MODE {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE,
  };

  FileDescriptor(int fd);
  FileDescriptor(const std::string& filename, MODE mode);
  ~FileDescriptor() override {}

  int fd() const { return fd_; }
  int Read(void* buffer, int nbytes) override;
  int Write(const void* buf, int nbytes) const override;
  int Close() override;
  int Seek(int32 offset);
 
 public:
  int fd_;
};

}  // namespace IO

#endif  // FILE_DESCRIPTOR_H_
