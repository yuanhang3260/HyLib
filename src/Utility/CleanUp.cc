#include "Utility/CleanUp.h"

namespace Utility {

CleanUp::CleanUp(std::function<void()> clean_up) : clean_up_(clean_up) {}

CleanUp::~CleanUp() {
  if (clean_up_) {
    clean_up_();
  }
}

void CleanUp::clear() {
  clean_up_ = std::function<void()>();
}

}  // namespace Utility
