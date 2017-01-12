#include <mutex>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdexcept>

#include "Log.h"

namespace {
std::mutex log_mutex;
}

void LogToINFO(const char* file, int line, const char* func,
               const char* error_msg, ...) {
  std::unique_lock<std::mutex> lock(log_mutex);
  printf("[%s.%d] ", file, line);
  va_list args;
  va_start(args, error_msg);
  vfprintf(stdout, error_msg, args);
  va_end(args);
  fprintf(stdout, "\n");
}

void LogToERROR(const char* file, int line, const char* func,
                const char* error_msg, ...) {
  std::unique_lock<std::mutex> lock(log_mutex);
  fprintf(stderr, "\033[1;31m[\033[0m%s.%d] ",
          file, line);
  va_list args;
  va_start(args, error_msg);
  vfprintf(stderr, error_msg, args);
  va_end(args);
  fprintf(stderr, "\n");
}

void LogToFATAL(const char* file, int line, const char* func,
                const char* error_msg, ...) {
  std::unique_lock<std::mutex> lock(log_mutex);
  fprintf(stderr, "\033[1;31m[\033[0m%s.%d] ",
          file, line);
  va_list args;
  va_start(args, error_msg);
  vfprintf(stderr, error_msg, args);
  va_end(args);
  fprintf(stderr, "\n");
  throw std::runtime_error("LogFATAL");
}

void debug(int i) {
  std::cout << "debug " << i << std::endl;
}
