/*
   ____                                            _       __ _
  / ___|___  _ __ ___  _ __ ___   ___  _ __     __| | ___ / _(_)_ __   ___  ___
 | |   / _ \| '_ ` _ \| '_ ` _ \ / _ \| '_ \   / _` |/ _ \ |_| | '_ \ / _ \/ __|
 | |__| (_) | | | | | | | | | | | (_) | | | | | (_| |  __/  _| | | | |  __/\__ \
  \____\___/|_| |_| |_|_| |_| |_|\___/|_| |_|  \__,_|\___|_| |_|_| |_|\___||___/

Author: Paolo Bosetti, 2026
*/

#pragma once

#include <iostream>
#include <optional>
#include <sstream>
#include <unistd.h>

// Semantic versioning
#define CNCPP_VERSION "0.1.0"

// Width of coordinate fileds
#define CNCPP_NUMBERS_WIDTH "9"

// Common types
using data_t = double; // Type for data values
using opt_data_t = std::optional<data_t>;
using opt_int_t = std::optional<int>;

namespace cncpp {

class Object {
public:
  virtual ~Object() = default;

  virtual std::string desc(bool colored = true) const = 0;

  /**
   * @brief Streams a textual representation of the point.
   * @param os Output stream.
   * @param v Point to print.
   * @return Reference to @p os.
   */
  friend std::ostream &operator<<(std::ostream &os, const Object &v);
};

inline std::ostream &operator<<(std::ostream &os, const Object &v) {
  bool is_terminal = (&os == &std::cout && isatty(STDOUT_FILENO)) ||
                     (&os == &std::cerr && isatty(STDERR_FILENO));
  os << v.desc(is_terminal);
  return os;
}

} // namespace cncpp
