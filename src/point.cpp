/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/

*/

#include "point.hpp"
#include <cmath>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;
using namespace cncpp;
using namespace fmt;
using col_t = optional<color>;

static string coord_str(opt_data_t const &coord, col_t const &color = nullopt);

// LIFECYCLE ===================================================================
Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

string Point::desc(bool colored) const {
  stringstream ss;
  ss << "[" << coord_str(_x, colored ? col_t(color::red) : nullopt) << ", "
     << coord_str(_y, colored ? col_t(color::green) : nullopt) << ", "
     << coord_str(_z, colored ? col_t(color::blue) : nullopt) << "]";
  return ss.str();
}

void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}

// OPERATORS/OPERATIONS ========================================================
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

// ACCESSORS ===================================================================

std::vector<data_t> Point::vec() const {
  if (!is_complete()) {
    throw runtime_error("Point is not complete, can't convert to vector");
  }
  return {_x.value(), _y.value(), _z.value()};
}

// UTILITY FUNCTIONS ===========================================================

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

#ifdef CNCPP_TEST_MAIN
#include <iostream>

int main() {
  Point p1(1.0, 2.0, 3.0);
  cout << "p1: " << p1 << endl;
  Point p2(4.0, 5.0);
  cout << "p2: " << p2 << endl;
  p2.modal(p1);
  cout << "p2 after modal with p1: " << p2 << endl;
  Point p3 = p1 + p2;
  cout << "p3 (p1 + p2): " << p3 << endl;
  cout << "p3 length: " << p3.length() << endl;
  cerr << "p3 delta p1: " << p3.delta(p1) << endl;
  return 0;
}

#endif // CNCPP_TEST_MAIN