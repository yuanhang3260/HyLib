#include "Base/Log.h"
#include "Strings/Convert.h"

namespace Strings {

bool ToInt32Safe(const std::string& str, int32 *v) {
  try {
    *v = std::stoi(str);
  }
  catch (std::exception& err) {
    LogERROR("Can't parse \"%s\" as int32 value", str.c_str());
    return false;
  }
  return true;
}

bool ToUInt32Safe(const std::string& str, uint32 *v) {
  try {
    *v = std::stoul(str);
  }
  catch (std::exception& err) {
    LogERROR("Can't parse \"%s\" as uint32 value", str.c_str());
    return false;
  }
  return true;
}

bool ToInt64Safe(const std::string& str, int64 *v) {
  try {
    *v = std::stoll(str);
  }
  catch (std::exception& err) {
    LogERROR("Can't parse \"%s\" as int64 value", str.c_str());
    return false;
  }
  return true;
}

bool ToUInt64Safe(const std::string& str, uint64 *v) {
  try {
    *v = std::stoull(str);
  }
  catch (std::exception& err) {
    LogERROR("Can't parse \"%s\" as uint64 value", str.c_str());
    return false;
  }
  return true;
}

bool ToDoubleSafe(const std::string& str, double *v) {
  try {
    *v = std::stod(str);
  }
  catch (std::exception& err) {
    LogERROR("Can't parse \"%s\" as double value", str.c_str());
    return false;
  }
  return true;
}

}  // namespace Strings
