#ifndef __FILE_DESCRIPTOR_H__
#define __FILE_DESCRIPTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

#include "FileDescriptorInterface.h"

namespace IO {

class FileDescriptor : public FileDescriptorInterface {
 public:
  enum MODE {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE,
  };

  FileDescriptor() = default;
  FileDescriptor(std::string filename, MODE mode);
  virtual ~FileDescriptor() {};

  virtual int Read(void* buffer, const int nbytes) const;
  virtual int Write(const void* buf, const int nbytes) const;
};

}  // namespace IO

#endif  /* __FILE_DESCRIPTOR_H__ */
