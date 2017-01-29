#include "FileDescriptorInterface.h"

namespace IO {

int FileDescriptorInterface::Close() {
  return 0;
}

FileDescriptorInterface::~FileDescriptorInterface() {
  if (!closed_) {
    Close();
  }
}

}  // namespace IO
