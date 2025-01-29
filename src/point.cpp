/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/

Point class implementation
Author: Paolo Bosetti, 2024
*/
#include "point.hpp"
#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

using namespace cncpp;
using namespace fmt;

Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}

void Point::modal(Point p) {
  if (p._x && !_x) {
    _x = p._x;
  }
  if (p._y && !_y) {
    _y = p._y;
  }
  if (p._z && !_z) {
    _z = p._z;
  }
}

Point Point::operator+(const Point& other) const {
  if (!complete() || !other.complete())
    throw CNCError("(+) Point is not complete", this);
  return Point(
    _x.value() + other._x.value(), 
    _y.value() + other._y.value(), 
    _z.value() + other._z.value());
}

Point Point::delta(Point p) {
  if (!complete() || !p.complete())
    throw CNCError("(delta) Point is not complete", this);
  Point out(_x.value_or(0) - p._x.value_or(0),
            _y.value_or(0) - p._y.value_or(0),
            _z.value_or(0) - p._z.value_or(0));
  return out;
}

data_t Point::length() const {
  if (!complete())
    throw CNCError("(length) Point is not complete", this);
  return sqrt(_x.value_or(0) * _x.value_or(0) +
              _y.value_or(0) * _y.value_or(0) +
              _z.value_or(0) * _z.value_or(0));
}

string coord_str(opt_data_t coord, optional<fmt::color> color = nullopt) {
  string str;
  if (coord && color)
    str = format("{:" NUMBERS_WIDTH ".3f}",
                 styled(coord.value(), fmt::fg(color.value())));
  else if (coord)
    str = format("{:" NUMBERS_WIDTH ".3f}", coord.value());
  else
    str = format("{:>" NUMBERS_WIDTH "}", "-");
  return str;
}

string Point::desc(bool colored) const {
  stringstream ss;
  using col_t = optional<fmt::color>;
  ss << "[" << coord_str(_x, colored ? col_t(color::red) : nullopt) << ", "
     << coord_str(_y, colored ? col_t(color::green) : nullopt) << ", "
     << coord_str(_z, colored ? col_t(color::blue) : nullopt) << "]";
  return ss.str();
}

vector<data_t> Point::vec() const {
  if (!complete())
    throw CNCError("(vec) Point is not complete", this);
  return vector<data_t>{_x.value(), _y.value(), _z.value()};
}

/*
  _____         _
 |_   _|__  ___| |_
   | |/ _ \/ __| __|
   | |  __/\__ \ |_
   |_|\___||___/\__|

*/

#ifdef POINT_MAIN
#include <iostream>

int main() {
  Point p1(0, 0, 0);
  Point p2(1);
  Point p3(nullopt, 2, 3);

  cout << "Before modal:" << endl;
  cout << p1.desc();
  cout << endl;
  cout << p2.desc();
  cout << endl;
  cout << p3.desc();
  cout << endl;

  p2.modal(p1);
  p3.modal(p2);
  p3.z(30);

  cout << "After modal:" << endl;
  cout << p1.desc();
  cout << endl;
  cout << p2.desc();
  cout << endl;
  cout << p3.desc();
  cout << endl;

  cout << "Length of p3: " << p3.length() << endl;

  Point p4{4, 3, 2};

  cout << "Delta of p4 and p3:" << endl;
  cout << p4.delta(p3).desc();
  cout << endl;

  cout << "Vector of p4:" << endl;
  cout << format("{}", p4.vec()) << endl;

  Point p5(1, 2);
  try {
    cout << "Vector of p5:" << endl;
    cout << format("{}", p5.vec()) << endl;
  } catch (CNCError &e) {
    cerr << "Error: " << e.who() << ": " << e.what() << endl;
  }

  return 0;
}

#endif