#ifndef FILE_DESCRIPTOR_INTERFACE_H_
#define FILE_DESCRIPTOR_INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory>

#include "Base/MacroUtils.h"

namespace IO {

class FileDescriptorInterface {
 public:
  FileDescriptorInterface() = default;
  FORBID_COPY_AND_ASSIGN(FileDescriptorInterface);

  virtual ~FileDescriptorInterface();

  bool closed() { return closed_; }

  virtual int Read(void* buffer, int nbytes) const = 0;
  virtual int Write(const void* buf, int nbytes) const = 0;
  virtual int Close();

 protected:
  bool closed_ = true;
};

}  // namespace IO

#endif  // FILE_DESCRIPTOR_INTERFACE_H_
