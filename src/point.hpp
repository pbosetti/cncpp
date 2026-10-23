/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/


Represents a 3-D coordinate position in space with
optional components and basic operations
*/

#pragma once
#include "defines.hpp"
#include <string>
#include <vector>

namespace cncpp {

// class names use CamelCase
// variables and functions use snake_case
/**
 * @brief Represents a point in 3-D space with optional coordinates.
 *
 * A coordinate can be unset (std::nullopt) and later completed through
 * mutators or modal updates. Partial points only come from G-code parsing
 * (e.g. "G01 X10" leaves Y and Z unset until modal() fills them in).
 * Arithmetic operations require complete points.
 */
class Point : public Object {

public:
  // LIFECYCLE =================================================================
  /**
   * @brief Constructs a point with optional x, y and z components.
   * @param x Optional x coordinate.
   * @param y Optional y coordinate.
   * @param z Optional z coordinate.
   */
  Point(opt_data_t x = std::nullopt, opt_data_t y = std::nullopt,
        opt_data_t z = std::nullopt);

  /**
   * @brief Returns a string representation of the point.
   * @param colored If true, enable colored formatting in the output string.
   * @return String description of the point.
   */
   std::string desc(bool colored = true) const override;

  /**
   * @brief Resets all coordinates to std::nullopt.
   */
  void reset();

  // OPERATORS/OPERATIONS ======================================================

  /**
   * @brief Computes the Euclidean length of the point vector.
   * @return Vector norm of the point.
   */
  data_t length() const;

  /**
   * @brief Applies modal behavior by filling missing coordinates from another point.
   * @param other Source point for modal coordinate values.
   */
  void modal(Point const &other);

  /**
   * @brief Adds two points component-wise.
   * @param other Point to add.
   * @return Sum of this point and @p other.
   */
  Point operator+(Point const &other) const; // Point p3 = p1 + p2;

  /**
   * @brief Subtracts two points component-wise.
   * @param other Point to subtract.
   * @return Difference of this point and @p other.
   */
  Point operator-(Point const &other) const; // Point d = p1 - p2;

  /**
   * @brief Checks whether all coordinates are set.
   * @return True if x, y and z are all present.
   */
  bool is_complete() const { return _x && _y && _z; }

  // ACCESSORS =================================================================
  /**
   * @brief Gets the x coordinate.
   * @return Value of x.
   */
  data_t x() const { return _x.value(); }

  /**
   * @brief Gets the y coordinate.
   * @return Value of y.
   */
  data_t y() const { return _y.value(); }

  /**
   * @brief Gets the z coordinate.
   * @return Value of z.
   */
  data_t z() const { return _z.value(); }

  /**
   * @brief Sets the x coordinate.
   * @param v New x value.
   * @return Updated x value.
   */
  data_t x(data_t v) { return (_x = v).value(); }

  /**
   * @brief Sets the y coordinate.
   * @param v New y value.
   * @return Updated y value.
   */
  data_t y(data_t v) { return (_y = v).value(); }

  /**
   * @brief Sets the z coordinate.
   * @param v New z value.
   * @return Updated z value.
   */
  data_t z(data_t v) { return (_z = v).value(); }

  /**
   * @brief Returns coordinates as a contiguous vector.
   * @return Vector containing point coordinates.
   */
  std::vector<data_t> vec() const;



private:
  // Optional x coordinate component.
  opt_data_t _x = std::nullopt;
  // Optional y coordinate component.
  opt_data_t _y = std::nullopt;
  // Optional z coordinate component.
  opt_data_t _z = std::nullopt;
};

} // namespace cncpp