#include <chrono>
#include <ctime>
#include <uuid/uuid.h>

#include "Utility/Uuid.h"

namespace UUID {

uint64 TimeStampID() {
  auto period = std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::system_clock::now().time_since_epoch());
  return period.count();
}

std::string GenerateUUID() {
  uuid_t uuid;
  uuid_generate(uuid);
  char buf[2 * sizeof(uuid)];
  for (uint32 i = 0; i < sizeof(uuid); i++) {
    sprintf(buf + 2 * i, "%02x", uuid[i]);
  }

  return std::string(buf, 2 * sizeof(uuid));
}

}  // namespace Utility
