#ifndef PTR_UTILS_H_
#define PTR_UTILS_H_

#include <memory>

namespace ptr {

template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args) {
  return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

}

#endif  // PTR_UTILS_H_
