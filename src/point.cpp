/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/

 Implementation
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
using col_t = optional<color>;

// FORWARD DECLARATIONS --------------------------------------------------------
static string coord_str(opt_data_t const &coord, col_t const &color = nullopt);



// LIFECYCLE -------------------------------------------------------------------
Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

// descrition like: [100.0, 200.0, 123.2]
string Point::desc(bool col) const {
  stringstream ss;
  ss << "[" << coord_str(_x, col ? col_t(color::red) : nullopt) << ", "
     << coord_str(_y, col ? col_t(color::green) : nullopt) << ", "
     << coord_str(_z, col ? col_t(color::blue) : nullopt) << "]";

  return ss.str();
}

void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}


// OPERATORS -------------------------------------------------------------------
Point Point::operator+(Point const &other) const {
  if (!is_complete() || !other.is_complete())
    throw CNCError("Points are not complete", this);

  Point out(_x.value() + other._x.value(), _y.value() + other._y.value(),
            _z.value() + other._z.value());
  return out;
}

Point& Point::operator=(Point const &other) {
  if (this != &other) {
    _x = other._x;
    _y = other._y;
    _z = other._z;
  }
  return *this;
}


// METHODS ---------------------------------------------------------------------
Point Point::delta(Point const &other) {
  if (!is_complete() || !other.is_complete())
    throw CNCError("Points are not complete", this);

  Point out(_x.value() - other._x.value(), _y.value() - other._y.value(),
            _z.value() - other._z.value());
  return out;
}

void Point::modal(Point const &p) {
  if (p._x && !_x)
    _x = p._x;
  if (p._y && !_y)
    _y = p._y;
  if (p._z && !_z)
    _z = p._z;
}

data_t Point::length() const {
  if (!is_complete())
    throw CNCError("Point is not complete", this);

  return sqrt(_x.value() * _x.value() + _y.value() * _y.value() +
              _z.value() * _z.value());
}


// ACCESSORS -------------------------------------------------------------------
vector<data_t> Point::vec() const {
  if (!is_complete())
  throw CNCError("Point is not complete", this);
  return vector<data_t>{_x.value(), _y.value(), _z.value()};
}


/*
_   _ _   _ _ _ _   _           
| | | | |_(_) (_) |_(_) ___  ___ 
| | | | __| | | | __| |/ _ \/ __|
| |_| | |_| | | | |_| |  __/\__ \
\___/ \__|_|_|_|\__|_|\___||___/

*/

// Utility function to format coordinates (static = only visible here)
static string coord_str(opt_data_t const &coord, col_t const &color) {
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