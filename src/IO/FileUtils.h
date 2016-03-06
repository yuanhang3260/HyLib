#ifndef FILE_UTILS_
#define FILE_UTILS_

#include <string>

namespace IO {

class FileUtils {
 public:
  static std::string GetAbstractPath(std::string filename);
};

}  // namespace IO

#endif  /* FILE_UTILS_ */