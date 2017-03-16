#ifndef UTILITY_INFINITE_BUFFER_
#define UTILITY_INFINITE_BUFFER_

#include "Base/BaseTypes.h"
#include "Utility/BufferInterface.h"

namespace Utility {

// Infinite buffer automatically increases. This class is not thread-safe.
class InfiniteBuffer : public BufferInterface {
 public:
  InfiniteBuffer();
  explicit InfiniteBuffer(uint32 init_capacity);
  ~InfiniteBuffer();

  // Write data into buffer. Return actual size written.
  uint32 Write(const byte* data, uint32 size) override;

  // Read data from buffer. Return actual size read.
  uint32 Read(byte* buf, uint32 size) override;

  void Clear() override;

 private:
  byte* buffer_ = nullptr;

  uint32 start_ = 0;
  uint32 end_ = 0;
};

}

#endif  // UTILITY_INFINITE_BUFFER_
