/*
   ____                                            _       __
  / ___|___  _ __ ___  _ __ ___   ___  _ __     __| | ___ / _|___
 | |   / _ \| '_ ` _ \| '_ ` _ \ / _ \| '_ \   / _` |/ _ \ |_/ __|
 | |__| (_) | | | | | | | | | | | (_) | | | | | (_| |  __/  _\__ \
  \____\___/|_| |_| |_|_| |_| |_|\___/|_| |_|  \__,_|\___|_| |___/

*/
#pragma once
#include <iostream>
#include <optional>
#include <unistd.h>

// DEFINES =====================================================================
#define CNCPP_NUMBERS_WIDTH "9"

// TYPEDEFS AND ALIASES ========================================================
using data_t = double;
using opt_data_t = std::optional<data_t>;
using opt_int_t = std::optional<int>;

// NAMESPACE ===================================================================
namespace cncpp {
/**
 * @brief A base class for objects that can be described as strings, optionally
 * with color.
 *
 */
class Object {
public:
  /**
   * @brief Virtual destructor for the Object class.
   *
   * This ensures that derived classes can clean up resources properly when
   * deleted through a pointer to Object, as in:
   * 
   * ```
   * Object *o = new Point(); 
   * // ...
   * delete o;
   * ```
   * The derived class destructor will be called first, followed by the
   * Object's own destructor (in this case, the default destructor).
   */
  virtual ~Object() = default;

  /**
   * @brief Returns a string representation of the object.
   * @param colored Whether to include color codes in the output.
   * @return A string representation of the object.
   */
  virtual std::string desc(bool colored = true) const = 0;

  /**
   * @brief Overloads the << operator to print an Object to an output stream.
   * @param os The output stream.
   * @param o The Object to print.
   * @return The output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Object &o);
};

/**
 * @brief Overloads the << operator to print an Object to an output stream.
 * @param os The output stream.
 * @param o The Object to print.
 * @return The output stream.
 */
inline std::ostream &operator<<(std::ostream &os, const Object &o) {
  bool is_terminal = (&os == &std::cout && isatty(STDOUT_FILENO)) ||
                     (&os == &std::cerr && isatty(STDERR_FILENO));
  os << o.desc(is_terminal);
  return os;
}

} // namespace cncpp