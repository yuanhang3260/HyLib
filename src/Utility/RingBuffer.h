#ifndef UTILITY_RING_BUFFER_
#define UTILITY_RING_BUFFER_

#include "Base/BaseTypes.h"
#include "Utility/BufferInterface.h"

namespace Utility {

// Circular buffer which has a fixed capacity. This class is not thread-safe.
class RingBuffer : public BufferInterface {
 public:
  RingBuffer();
  explicit RingBuffer(uint32 capacity);
  ~RingBuffer();

  // Write data into buffer. Return actual size written.
  uint32 Write(const byte* data, uint32 size) override;

  // Read data from buffer. Return actual size read.
  uint32 Read(byte* buf, uint32 size) override;

  void Clear() override;

  uint32 free_space() { return capacity_ - size_; }

 private:
  byte* buffer_ = nullptr;

  uint32 start_ = 0;
  uint32 end_ = 0;
};

}

#endif  // UTILITY_RING_BUFFER_
