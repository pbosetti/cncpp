/*
Point class
*/

#include "point.hpp"
#include <cmath>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace fmt;
using col_t = optional<color>;

static string coord_str(opt_data_t const &coord, col_t const &color = nullopt);

// LIFECYCLE
Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

string Point::desc(bool colored) const {
  stringstream ss;
  ss << "[" << coord_str(_x, col_t(color::red)) << ", "
     << coord_str(_y, col_t(color::green)) << ", "
     << coord_str(_z, col_t(color::blue)) << "]";
  return ss.str();
}

void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}

// OPERATORS/OPERATIONS
Point Point::delta(Point const &o) const {
  if (!is_complete() || !o.is_complete()) {
    throw runtime_error("Points are not complete, can't do delta");
  }
  Point out(_x.value() - o._x.value(), _y.value() - o._y.value(),
            _z.value() - o._z.value());
  return out;
}

data_t Point::length() const {
  if (!is_complete()) {
    throw runtime_error("Point is not complete, can't compute length");
  }
  return hypot(_x.value(), _y.value(), _z.value());
}

void Point::modal(Point const &o) {
  if (o._x && !_x)
    _x = o._x;
  if (o._y && !_y)
    _y = o._y;
  if (o._z && !_z)
    _z = o._z;
}

Point &Point::operator=(Point const &o) {
  _x = o._x;
  _y = o._y;
  _z = o._z;
  return *this;
}

Point Point::operator+(Point const &o) const {
  if (!is_complete() || !o.is_complete()) {
    throw runtime_error("Points are not complete, can't do the sum");
  }
  Point out(_x.value() + o._x.value(), _y.value() + o._y.value(),
            _z.value() + o._z.value());
  return out;
}

// ACCESSORS

// UTILITY FUNCTIONS

static string coord_str(opt_data_t const &coord, col_t const &color) {
  string str;
  if (coord && color) {
    str = format("{:" CNCPP_NUMBERS_WIDTH ".3f}",
                 styled(coord.value(), fg(color.value())));
  } else if (coord) {
    str = format("{:" CNCPP_NUMBERS_WIDTH ".3f}", coord.value());
  } else {
    str = format("{:>" CNCPP_NUMBERS_WIDTH "}", "-");
  }
  return str;
}
