#include <string.h>

#include "Base/Log.h"
#include "Base/Utils.h"
#include "Utility/RingBuffer.h"

namespace Utility {

namespace {
uint32 kDefaultSize = 4096;
}

RingBuffer::RingBuffer() : capacity_(kDefaultSize) {
  buffer_ = new byte[capacity_];
}

RingBuffer::RingBuffer(uint32 capacity) : capacity_(capacity) {
  buffer_ = new byte[capacity_];
}

RingBuffer::~RingBuffer() {
  if (buffer_) {
    delete[] buffer_;
  }
}

uint32 RingBuffer::Write(const byte* data, uint32 size) {
  if (!data) {
    LogERROR("Bad argument - data is nullptr.");
    return 0;
  }

  uint32 size_to_write = Utils::Min(size, capacity_ - size_);
  if (size_to_write <= 0) {
    return size_to_write;
  }

  if (start_ <= end_) {
    if (capacity_ - end_ >= size_to_write) {
      memcpy(buffer_ + end_, data, size_to_write);
      end_ += size_to_write;
    } else {
      memcpy(buffer_ + end_, data, capacity_ - end_);
      memcpy(buffer_, data + (capacity_ - end_),
             size_to_write - (capacity_ - end_));
      end_ = size_to_write - (capacity_ - end_);
    }
  } else {
    memcpy(buffer_ + end_, data, size_to_write);
    end_ += size_to_write;
  }

  size_ += size_to_write;
  return size_to_write;
}

uint32 RingBuffer::Read(byte* buf, uint32 size) {
  if (!buf) {
    LogERROR("Bad argument - receive buffer is nullptr.");
    return 0;
  }

  uint32 size_to_read = Utils::Min(size, size_);
  if (size_to_read <= 0) {
    return size_to_read;
  }

  if (start_ < end_) {
    memcpy(buf, buffer_ + start_, size_to_read);
    start_ += size_to_read;
  } else {
    if (capacity_ - start_ >= size_to_read) {
      memcpy(buf, buffer_ + start_, size_to_read);
      start_ += size_to_read;
    } else {
      memcpy(buf, buffer_ + start_, capacity_ - start_);
      memcpy(buf + capacity_ - start_, buffer_,
             size_to_read - (capacity_ - start_));
      start_ = size_to_read - (capacity_ - start_);
    }
  }

  size_ -= size_to_read;
  return size_to_read;
}

void RingBuffer::Clear() {
  if (buffer_) {
    delete[] buffer_;
  }

  buffer_ = nullptr;
  size_ = 0;
  start_ = 0;
  end_ = 0;
}

}  // namsspace Utility
