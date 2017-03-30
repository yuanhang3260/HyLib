#include <string.h>

#include "Base/Log.h"
#include "Base/Utils.h"
#include "Utility/InfiniteBuffer.h"

namespace Utility {

namespace {
uint32 kDefaultSize = 4096;
}

InfiniteBuffer::InfiniteBuffer() : BufferInterface(kDefaultSize) {
  buffer_ = new byte[capacity_];
}

InfiniteBuffer::InfiniteBuffer(uint32 capacity) : BufferInterface(capacity) {
  buffer_ = new byte[capacity_];
}

InfiniteBuffer::~InfiniteBuffer() {
  if (buffer_) {
    delete[] buffer_;
  }
}

uint32 InfiniteBuffer::Write(const byte* data, uint32 size) {
  if (!data) {
    LogERROR("Bad argument - data is nullptr.");
    return 0;
  }

  if (size < 0) {
    LogERROR("Invalid Write request: size = 0");
    return 0;
  }

  if (end_ + size > capacity_) {
    // Double the buffer space.
    uint32 new_buffer_size = Utils::Max(size_ + size, capacity_ * 2);
    byte* new_buffer = new byte[new_buffer_size];
    if (!new_buffer) {
      LogERROR("Expand buffer failed");
      return 0;
    }
    memcpy(new_buffer, buffer_ + start_, size_);
    delete[] buffer_;
    buffer_ = new_buffer;
    capacity_ = new_buffer_size;
    start_ = 0;
    end_ = size_;
  }

  memcpy(buffer_ + end_, data, size);
  size_ += size;
  end_  += size;
  return size;
}

uint32 InfiniteBuffer::Read(byte* buf, uint32 size) {
  if (!buf) {
    LogERROR("Bad argument - receive buffer is nullptr.");
    return 0;
  }

  if (size < 0) {
    LogERROR("Invalid Read request: size = 0");
    return 0;
  }

  uint32 size_to_read = Utils::Min(size, size_);
  memcpy(buf, buffer_ + start_, size_to_read);
  size_ -= size_to_read;
  start_ += size_to_read;
  return size_to_read;
}

void InfiniteBuffer::Clear() {
  if (buffer_) {
    delete[] buffer_;
  }

  buffer_ = nullptr;
  capacity_ = 0;
  size_ = 0;
  start_ = 0;
  end_ = 0;
}

void InfiniteBuffer::PrintBuffer() const {
  for (uint32 i = 0; i < size_; i++) {
    printf("%c", buffer_[start_ + i]);
  }
  printf("\n");
}

}  // namsspace Utility
