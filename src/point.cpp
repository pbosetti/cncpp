/*
Point class implementation
*/

#include "point.hpp"
#include <sstream>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;
using namespace cncpp;
using namespace fmt;
using col_t = optional<color>;

// UTILITY FUNCTIONS ===========================================================
// Foreward declaration of a utility function:
// this one formats a coordinate value, optionally coloring it
static string coord_str(opt_data_t const &coord, col_t const &color=nullopt);

// LIFECYCLE====================================================================
Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

// return something like [ 1, 2, 3], optionally in colors
std::string Point::desc(bool colored) const {
  stringstream ss;
  ss << "["
     << coord_str(_x, colored ? col_t(color::red) : nullopt) << ","
     << coord_str(_y, colored ? col_t(color::green) : nullopt) << ","
     << coord_str(_z, colored ? col_t(color::blue) : nullopt) << "]";
  return ss.str();
}

void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}


// INTERFACES ==================================================================
data_t Point::length() const {
  if (!is_complete()) {
    throw runtime_error("Point is not complete, can't compute length");
  }
  return hypot(_x.value(), _y.value(), _z.value());
}


void Point::modal(Point const &o) {
  if (o._x && !_x) _x = o._x;
  if (o._y && !_y) _y = o._y;
  if (o._z && !_z) _z = o._z;
}


Point Point::operator+(Point const &o) const {
  if (!is_complete() || !o.is_complete()) {
    throw runtime_error("Points are not complete, can't add");
  }
  Point out(
    _x.value() + o._x.value(),
    _y.value() + o._y.value(),
    _z.value() + o._z.value()
  );
  return out;
}

Point Point::operator-(Point const &o) const {
  if (!is_complete() || !o.is_complete()) {
    throw runtime_error("Points are not complete, can't subtract");
  }
  Point out(
    _x.value() - o._x.value(),
    _y.value() - o._y.value(),
    _z.value() - o._z.value()
  );
  return out;
}


// ACCESSORS ===================================================================

vector<data_t> Point::vec() const {
  if (!is_complete()) {
    throw runtime_error("Point is not complete, can't make a vector");
  }
  return {_x.value(), _y.value(), _z.value()};
}



// UTILITY FUNCTIONS (IMPLEMENTATION) ==========================================

/**
 * @brief Formats a coordinate value, optionally coloring it.
 * 
 * Uses the fmt library to format the coordinate value with a fixed width and 
 * precision, using a color if provided. If the coordinate is not set, it 
 * returns a placeholder string.
 * 
 * See https://fmt.dev/latest/index.html for more information on the fmt 
 * library.
 * 
 * @param coord The coordinate value to format.
 * @param color The color to use for formatting (optional).
 * @return string The formatted coordinate value.
 */
static string coord_str(opt_data_t const &coord, col_t const &color) {
  string str;
  
  // ONLY WORKS WITH LITERAL STRINGS!!!
  // "{:" CNCPP_NUMBERS_WIDTH ".3f}"
  // -> "{:" "9" ".3f}"
  // -> "{:9.3f}"
  if (coord && color) {
    auto val = coord.value();
    auto col = color.value();
    str = fmt::format("{:" CNCPP_NUMBERS_WIDTH ".3f}", styled(val, fg(col)));
  } else if (coord) {
    auto val = coord.value();
    // max fit for -9999.000
    str = fmt::format("{:" CNCPP_NUMBERS_WIDTH ".3f}", val);
  } else {
    str = fmt::format("{:>" CNCPP_NUMBERS_WIDTH "}", "-");
  }
  return str;
}


// TEST MAIN ===================================================================
// There can be only one main() function in a C++ program, so this is only
// compiled if CNCPP_TEST_POINT is defined. This allows for testing the Point
// class in isolation, and yet compiling point.cpp into a larger program 
// without conflicts.
#ifdef CNCPP_TEST_POINT

int main() {
  Point p1(1.0, 2.0, 3.0);
  cout << "p1: " << p1 << endl;

  Point p2(4.0, 5.0);
  cout << "p2: " << p2 << endl;

  p2.modal(p1);
  cout << "p2 after modal: " << p2 << endl;

  auto p3 = p1 + p2;
  auto p4 = p2 - p1;

  cout << "p3: " << p3 << endl;
  cout << "p3 length: " << p3.length() << endl;
  cout << "p4: " << p4 << endl;

  return 0;
}



#endif
