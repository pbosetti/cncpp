/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/

Class representing a 3D point, with optional coordinates (x, y, z).
*/
#pragma once

#include "defines.hpp"
#include <string>
#include <vector>

namespace cncpp {

/**
 * @brief Represents a three-dimensional point with optional coordinates.
 *
 * A coordinate may be unset to represent an omitted axis, as can occur in a
 * modal G-code command. Operations that require all three coordinates are
 * available only when the point is complete.
 */
class Point : public Object {
public:
  // LIFECYCLE =================================================================

  /**
   * @brief Constructs a point with the supplied optional coordinates.
   * 
   * This class is the basic container for 3-d coordinates used by the CNC 
   * kernel
   * 
   * @param x The x-coordinate, or std::nullopt if it is unset.
   * @param y The y-coordinate, or std::nullopt if it is unset.
   * @param z The z-coordinate, or std::nullopt if it is unset.
   */
  Point(opt_data_t x = std::nullopt, opt_data_t y = std::nullopt,
        opt_data_t z = std::nullopt);

  /**
   * @brief Returns a string representation of the point.
   * @param colored Whether coordinate values should be formatted with colors.
   * @return The point formatted as three coordinates; unset coordinates are
   *         shown as placeholders.
   */
  std::string desc(bool colored = true) const override;

  /**
   * @brief Unsets all three coordinates.
   */
  void reset();

  // INTERFACES ================================================================

  /**
   * @brief Calculates the Euclidean distance from the origin to this point.
   * @return The point's length.
   * @throws std::runtime_error If any coordinate is unset.
   */
  data_t length() const;

  /**
   * @brief Fills this point's unset coordinates from another point.
   *
   * Coordinates already set on this point are left unchanged, and unset
   * coordinates on @p other are ignored.
   * @param other The point supplying values for this point's unset coordinates.
   */
  void modal(Point const &other);

  /**
   * @brief Adds the corresponding coordinates of two complete points.
   * @param other The point to add to this point.
   * @return A point containing the coordinate-wise sum.
   * @throws std::runtime_error If either point has an unset coordinate.
   */
  Point operator+(Point const &other) const;

  /**
   * @brief Subtracts the corresponding coordinates of another complete point.
   * @param other The point whose coordinates are subtracted from this point.
   * @return A point containing the coordinate-wise difference.
   * @throws std::runtime_error If either point has an unset coordinate.
   */
  Point operator-(Point const &other) const;

  /**
   * @brief Checks whether all three coordinates are set.
   * @return `true` if x, y, and z are set; otherwise, `false`.
   */
  bool is_complete() const { return _x && _y && _z; }

  // ACCESSORS =================================================================
  /**
   * @brief Gets the x-coordinate.
   * @return The x-coordinate.
   * @throws std::bad_optional_access If the x-coordinate is unset.
   */
  data_t x() const { return _x.value(); }

  /**
   * @brief Gets the y-coordinate.
   * @return The y-coordinate.
   * @throws std::bad_optional_access If the y-coordinate is unset.
   */
  data_t y() const { return _y.value(); }

  /**
   * @brief Gets the z-coordinate.
   * @return The z-coordinate.
   * @throws std::bad_optional_access If the z-coordinate is unset.
   */
  data_t z() const { return _z.value(); }

  /**
   * @brief Sets the x-coordinate.
   * @param v The new x-coordinate.
   * @return The assigned x-coordinate.
   */
  data_t x(data_t v) { return (_x = v).value(); }

  /**
   * @brief Sets the y-coordinate.
   * @param v The new y-coordinate.
   * @return The assigned y-coordinate.
   */
  data_t y(data_t v) { return (_y = v).value(); }

  /**
   * @brief Sets the z-coordinate.
   * @param v The new z-coordinate.
   * @return The assigned z-coordinate.
   */
  data_t z(data_t v) { return (_z = v).value(); }

  /**
   * @brief Returns the coordinates as a vector in x, y, z order.
   * @return A three-element vector containing the coordinates.
   * @throws std::runtime_error If any coordinate is unset.
   */
  std::vector<data_t> vec() const;

private:
  opt_data_t _x = std::nullopt;
  opt_data_t _y = std::nullopt;
  opt_data_t _z = std::nullopt;

}; // class Point

} // namespace cncpp