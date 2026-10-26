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

class Point : public Object {
public:
  // LIFECYCLE =================================================================

  Point(opt_data_t x = std::nullopt, opt_data_t y = std::nullopt,
        opt_data_t z = std::nullopt);

  std::string desc(bool colored = true) const;

  void reset();

  // INTERFACES ================================================================

  data_t length() const;

  // N01 G00 X100 Y100 Z200 -> Point(100, 100, 200)
  // N01 G00 Z100           -> Point(nullopt, nullopt, 100)
  // p2.modal(p1) -> [100, 100, 100]
  void modal(Point const &other);

  // Sum element by element the three coordinates
  Point operator+(Point const &other) const;

  // Calculate the projections on the three axes
  Point operator-(Point const &other) const;

  // Check if point is valid and complete
  bool is_complete() const { return _x && _y && _z; }

  // ACCESSORS =================================================================
  // getters (read values)
  data_t x() const { return _x.value(); }
  data_t y() const { return _y.value(); }
  data_t z() const { return _z.value(); }

  // setters
  data_t x(data_t v) { return (_x = v).value(); }
  data_t y(data_t v) { return (_y = v).value(); }
  data_t z(data_t v) { return (_z = v).value(); }

  std::vector<data_t> vec() const;

private:
  opt_data_t _x = std::nullopt;
  opt_data_t _y = std::nullopt;
  opt_data_t _z = std::nullopt;

}; // class Point

} // namespace cncpp