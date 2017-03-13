#ifndef UTILITY_RING_BUFFER_
#define UTILITY_RING_BUFFER_

#include "Base/BaseTypes.h"

namespace Utility {

// Circular buffer which has a fixed capacity. This class is not thread-safe.
class RingBuffer {
 public:
  RingBuffer();
  explicit RingBuffer(uint32 capacity);
  ~RingBuffer();

  // Write data into buffer. Return actual size written.
  uint32 Write(const byte* data, uint32 size);

  // Read data from buffer. Return actual size read.
  uint32 Read(byte* buf, uint32 size);

  void Clear();

  uint32 capacity() const { return capacity_; }
  uint32 size() const { return size_; }
  bool empty() const { return size_ == 0; }
  bool full() const { return size_ == capacity_; }

 private:
  uint32 capacity_ = 0;
  byte* buffer_ = nullptr;

  // Current buffered data size.
  uint32 size_ = 0;
  uint32 start_ = 0;
  uint32 end_ = 0;
};

}

#endif  // UTILITY_RING_BUFFER_
