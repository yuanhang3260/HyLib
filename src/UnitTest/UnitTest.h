#ifndef UNIT_TEST_
#define UNIT_TEST_

#include <iostream>
#include <exception>

class AssertError: public std::exception {
 public:
  virtual const char* what() const throw() {
    return "Assertion failure";
  }
};

class UnitTest {
 public:
  // TODO: move setup() and teardown() call upon each unit test.
  UnitTest() { }
  ~UnitTest() { }

  virtual void setup() {}
  virtual void teardown() {}

  template <typename T>
  void AssertEqual(T expect, T actual, std::string error_msg="") {
    if (expect != actual) {
      std::cerr << "[\033[1;31mAssertEqual\033[0m]: ";
      std::cerr << "Expect: " << expect << ", Actual: " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertEqual(T1 expect, T2 actual, std::string error_msg="") {
    return AssertEqual<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertNonEqual(T expect, T actual, std::string error_msg="") {
    if (expect == actual) {
      std::cerr << "[\033[1;31mAssertNonEqual\033[0m]: " << expect << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertNonEqual(T1 expect, T2 actual, std::string error_msg="") {
    return AssertEqual<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertGreater(T expect, T actual, std::string error_msg="") {
    if (expect <= actual) {
      std::cerr << "[\033[1;31mAssertGreater\033[0m]: "
                << expect  << " <= " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertGreater(T1 expect, T2 actual, std::string error_msg="") {
    return AssertGreater<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertLess(T expect, T actual, std::string error_msg="") {
    if (expect >= actual) {
      std::cerr << "[\033[1;31mAssertLess\033[0m]: "
                << expect  << " >= " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertLess(T1 expect, T2 actual, std::string error_msg="") {
    return AssertLess<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertGreaterEqual(T expect, T actual, std::string error_msg="") {
    if (expect < actual) {
      std::cerr << "[\033[1;31mAssertGreaterEqual\033[0m]: "
                << expect  << " < " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertGreaterEqual(T1 expect, T2 actual, std::string error_msg="") {
    return AssertGreaterEqual<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertLessEqual(T expect, T actual, std::string error_msg="") {
    if (expect > actual) {
      std::cerr << "[\033[1;31mAssertLessEqual\033[0m]: "
                << expect  << " > " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertLessEqual(T1 expect, T2 actual, std::string error_msg="") {
    return AssertLessEqual<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  void AssertTrue(bool condition, std::string error_msg="") {
    if (!condition) {
      std::cerr << "[\033[1;31mAssertTrue\033[0m]: False" << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  void AssertFalse(bool condition, std::string error_msg="") {
    if (condition) {
      std::cerr << "[\033[1;31mAssertFalse\033[0m]: True" << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }
};

#endif  /* UNIT_TEST_ */