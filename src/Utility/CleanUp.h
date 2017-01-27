#ifndef UTILITY_CLEANUP_H_
#define UTILITY_CLEANUP_H_

#include <functional>

namespace Utility {

class CleanUp {
 public:
  CleanUp(std::function<void()> clean_up);
  ~CleanUp();

  void clear();

 private:
  std::function<void()> clean_up_;
};

}  // namespace Utility

#endif /* UTILITY_CLEANUP_H_ */
