#ifndef UTILITY_UUID_H_
#define UTILITY_UUID_H_

#include <string>

#include "Base/BaseTypes.h"

namespace UUID {

// Return now since epoch in nano-seconds.
uint64 TimeStampID();

std::string GenerateUUID();

}  // namespace Utility

#endif /* UTILITY_UUID_H_ */
