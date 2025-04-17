/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/

*/

#include "point.hpp"
#include <exception>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <sstream>

using namespace std;
using namespace cncpp;
using namespace fmt;

Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

// Set all coords to nullopt (undefined)
void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}

void Point::modal(const Point &p) {
  if (p._x && !_x)
    _x = p._x;
  if (p._y && !_y)
    _y = p._y;
  if (p._z && !_z)
    _z = p._z;
}

Point Point::operator+(const Point &other) const {
  if (!is_complete() || !other.is_complete())
    throw CNCError("Points are not complete", this);

  Point out(_x.value() + other._x.value(), _y.value() + other._y.value(),
            _z.value() + other._z.value());
  return out;
}

Point Point::delta(const Point &other) {
  if (!is_complete() || !other.is_complete())
    throw CNCError("Points are not complete", this);

  Point out(_x.value() - other._x.value(), _y.value() - other._y.value(),
            _z.value() - other._z.value());
  return out;
}

data_t Point::length() const {
  if (!is_complete())
    throw CNCError("Point is not complete", this);

  return sqrt(_x.value() * _x.value() + _y.value() * _y.value() +
              _z.value() * _z.value());
}

using col_t = optional<color>;
static string coord_str(opt_data_t coord, col_t color = nullopt) {
  string str;
  if (coord && color) {
    str = format("{:" NUMBERS_WIDTH ".3f}",
                 styled(coord.value(), fg(color.value())));
  } else if (coord) {
    str = format("{:" NUMBERS_WIDTH ".3f}", coord.value());
  } else {
    str = format("{:>" NUMBERS_WIDTH "}", "-");
  }
  return str;
}

// descrition like: [100.0, 200.0, 123.2]
string Point::desc(bool col) const {
  stringstream ss;
  ss << "[" << coord_str(_x, col ? col_t(color::red) : nullopt) << ", "
     << coord_str(_y, col ? col_t(color::green) : nullopt) << ", "
     << coord_str(_z, col ? col_t(color::blue) : nullopt) << "]";

  return ss.str();
}

vector<data_t> Point::vec() const {
  if (!is_complete())
    throw CNCError("Point is not complete", this);
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
  Point p2(100);
  Point p3(nullopt, 20, 30);

  cout << "Before any change:" << endl;
  cout << "p1: " << p1.desc() << endl;
  cout << "p2: " << p2.desc() << endl;
  cout << "p3: " << p3.desc(false) << endl;

  // Modal test
  p2.modal(p1);
  p3.modal(p2);
  p3.z(100);

  cout << "After modal:" << endl;
  cout << "p1: " << p1.desc() << endl;
  cout << "p2: " << p2.desc() << endl;
  cout << "p3: " << p3.desc() << endl;

  cout << "Length of p3: " << p3.length() << endl;

  cout << "Delta of p1 and p3:" << endl;
  cout << p3.delta(p1).desc() << endl;

  Point p4(100, 20);

  try {
    cout << "p4 length: " << p4.length() << endl;
  } catch (CNCError &e) {
    cerr << "Error: " << e.what() << endl << "Raised by: " << e.who() << endl;
  } catch (exception &e) {
    cerr << "Unexpected error" << endl;
  }

  cout << "Vector from p3:" << endl;
  cout << format("{}", p3.vec()) << endl;

  cout << "Done." << endl;
  return 0;
}

#endif // POINT_MAIN