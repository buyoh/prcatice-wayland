#ifndef SRC_WRAPPER_UTIL_LOG_H__
#define SRC_WRAPPER_UTIL_LOG_H__

#include <iostream>

struct LOG_t {
  ~LOG_t() { std::cout << std::endl; }
};
#define VLOG \
  (LOG_t(),  \
   std::cout << __FILE__ << ' ' << __FUNCTION__ << " L" << __LINE__ << ": ")

#endif  // SRC_WRAPPER_UTIL_LOG_H__
