#ifndef STRINGS_CONVERT_H_
#define STRINGS_CONVERT_H_

#include <string>

#include "Base/BaseTypes.h"

namespace Strings {

bool ToInt32Safe(const std::string& str, int32 *v);
bool ToUInt32Safe(const std::string& str, uint32 *v);
bool ToInt64Safe(const std::string& str, int64 *v);
bool ToUInt64Safe(const std::string& str, uint64 *v);
bool ToDoubleSafe(const std::string& str, double *v);

}  // namespace Strings

#endif  // STRINGS_CONVERT_H_
