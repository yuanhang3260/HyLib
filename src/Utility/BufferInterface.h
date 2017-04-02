#ifndef UTILITY_BUFFER_INTERFACE_
#define UTILITY_BUFFER_INTERFACE_

#include "Base/BaseTypes.h"
#include "Utility/BufferInterface.h"

namespace Utility {

class BufferInterface {
 public:
  BufferInterface() = default;
  BufferInterface(uint32 capacity) { capacity_ = capacity; }
  virtual ~BufferInterface() {}

  // Write data into buffer. Return actual size written.
  virtual uint32 Write(const byte* data, uint32 size) = 0;

  // Read data from buffer. Return actual size read.
  virtual uint32 Read(byte* buf, uint32 size) = 0;

  // Clear the buffer.
  virtual void Clear() = 0;

  uint32 capacity() const { return capacity_; }
  uint32 size() const { return size_; }
  bool empty() const { return size_ == 0; }
  virtual bool full() const { return size_ == capacity_; }

 protected:
  // Total capacity.
  uint32 capacity_ = 0;
  // Current data size.
  uint32 size_ = 0;
};

}

#endif  // UTILITY_BUFFER_INTERFACE_
