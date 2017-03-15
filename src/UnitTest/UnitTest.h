#ifndef UNIT_TEST_
#define UNIT_TEST_

#include <iostream>
#include <exception>

#define AssertEqual(expect, actual, ...) \
  AssertEqual_impl(__FILE__, __LINE__, expect, actual, ## __VA_ARGS__)

#define AssertNonEqual(expect, actual, ...) \
  AssertNonEqual_impl(__FILE__, __LINE__, expect, actual, ## __VA_ARGS__)

#define AssertGreater(expect, actual, ...) \
  AssertGreater_impl(__FILE__, __LINE__, expect, actual, ## __VA_ARGS__)

#define AssertLess(expect, actual, ...) \
  AssertLess_impl(__FILE__, __LINE__, expect, actual, ## __VA_ARGS__)

#define AssertGreaterEqual(expect, actual, ...) \
  AssertGreaterEqual_impl(__FILE__, __LINE__, expect, actual, ## __VA_ARGS__)

#define AssertLessEqual(expect, actual, ...) \
  AssertLessEqual_impl(__FILE__, __LINE__, expect, actual, ## __VA_ARGS__)

#define AssertTrue(condition, ...) \
  AssertTrue_impl(__FILE__, __LINE__, condition, ## __VA_ARGS__)

#define AssertFalse(condition, ...) \
  AssertFalse_impl(__FILE__, __LINE__, condition, ## __VA_ARGS__)

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
  void AssertEqual_impl(const char* file, int line,
                        T expect, T actual, std::string error_msg="") {
    if (expect != actual) {
      std::cerr << "[\033[1;31mAssertEqual\033[0m "
                << file << "." << line << "]: ";
      std::cerr << "Expect: " << expect << ", Actual: " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertEqual_impl(const char* file, int line,
                        T1 expect, T2 actual, std::string error_msg="") {
    return AssertEqual_impl<T1>(file, line,
                                expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertNonEqual_impl(const char* file, int line,
                           T expect, T actual, std::string error_msg="") {
    if (expect == actual) {
      std::cerr << "[\033[1;31mAssertNonEqual\033[0m "
                << file << "." << line << "]: Both = " << actual;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertNonEqual_impl(const char* file, int line,
                           T1 expect, T2 actual, std::string error_msg="") {
    return AssertNonEqual_impl<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertGreater_impl(const char* file, int line,
                          T expect, T actual, std::string error_msg="") {
    if (expect <= actual) {
      std::cerr << "[\033[1;31mAssertGreater\033[0m "
                << file << "." << line << "]: "
                << expect  << " <= " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertGreater_impl(const char* file, int line,
                          T1 expect, T2 actual, std::string error_msg="") {
    return AssertGreater_impl<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertLess_impl(const char* file, int line,
                       T expect, T actual, std::string error_msg="") {
    if (expect >= actual) {
      std::cerr << "[\033[1;31mAssertLess\033[0m "
                << file << "." << line << "]: "
                << expect  << " >= " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertLess_impl(const char* file, int line,
                       T1 expect, T2 actual, std::string error_msg="") {
    return AssertLess_impl<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  template <typename T>
  void AssertGreaterEqual_impl(const char* file, int line,
                               T expect, T actual, std::string error_msg="") {
    if (expect < actual) {
      std::cerr << "[\033[1;31mAssertGreaterEqual\033[0m "
                << file << "." << line << "]: "
                << expect  << " < " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertGreaterEqual_impl(const char* file, int line,
                               T1 expect, T2 actual, std::string error_msg="") {
    return AssertGreaterEqual_impl<T1>(expect, static_cast<T1>(actual),
                                       error_msg);
  }

  template <typename T>
  void AssertLessEqual_impl(const char* file, int line,
                            T expect, T actual, std::string error_msg="") {
    if (expect > actual) {
      std::cerr << "[\033[1;31mAssertLessEqual\033[0m "
                << file << "." << line << "]: "
                << expect  << " > " << actual << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  template <typename T1, typename T2>
  void AssertLessEqual_impl(const char* file, int line,
                            T1 expect, T2 actual, std::string error_msg="") {
    return AssertLessEqual_impl<T1>(expect, static_cast<T1>(actual), error_msg);
  }

  void AssertTrue_impl(const char* file, int line,
                       bool condition, std::string error_msg="") {
    if (!condition) {
      std::cerr << "[\033[1;31mAssertTrue\033[0m "
                << file << "." << line << "]: Actual False" << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }

  void AssertFalse_impl(const char* file, int line,
                        bool condition, std::string error_msg="") {
    if (condition) {
      std::cerr << "[\033[1;31mAssertFalse\033[0m "
                << file << "." << line << "]: Actual True" << std::endl;
      std::cerr << "Error Message: " << error_msg << std::endl;
      throw AssertError();
    }
  }
};

#endif  /* UNIT_TEST_ */