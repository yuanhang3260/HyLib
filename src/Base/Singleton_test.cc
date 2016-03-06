#include <vector>
#include <set>

#include "Base/MacroUtils.h"
#include "UnitTest/UnitTest.h"
#include "Singleton.h"

class MyClass {
 public:
  void Foo() { printf("foo\n"); }
  int value() { return value_; }
 private:
  MyClass() = default;
  MyClass(int value) : value_(value) {}
  FORBID_COPY_AND_ASSIGN(MyClass);
  friend class Singleton<MyClass>;

  int value_;
};

template <>
MyClass* Singleton<MyClass>::CreateInstance() {
  printf("Specialized CreateInstance() for MyClass\n");
  MyClass* obj = new MyClass(5);
  return obj;
}

class SingletonTest: public UnitTest {
 public:
  void Test_Basic() {
    auto ptr = Singleton<MyClass>::get();
    ptr->Foo();
    AssertEqual(5, ptr->value());

    auto ptr1 = Singleton<MyClass>::get();
    auto ptr2 = Singleton<MyClass>::get();
    AssertEqual(ptr, ptr1);
    AssertEqual(ptr, ptr2);
  }
};


int main() {
  SingletonTest test;
  test.Test_Basic();

  std::cout << "\033[2;32mPassed ^_^\033[0m" << std::endl;
  return 0;
}