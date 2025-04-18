/*
  ____       _       _          _               
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___ 
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/
                                                
Represents a 3-D coordinate object, with optional components
and methods for calculating distances and projections
*/
#ifndef POINT_HPP
#define POINT_HPP

// INCLUDES AND DEFINES --------------------------------------------------------
#include "defines.hpp"
#include <vector>

// NAMESPACES AND CONSTANTS ----------------------------------------------------
using namespace std;

// Project namespace
namespace cncpp {

// The Point class, inheriting from Object
class Point : Object {
public:

  // LIFECYCLE -----------------------------------------------------------------
  Point(opt_data_t x = nullopt, opt_data_t y = nullopt, opt_data_t z = nullopt);
  string desc(bool colored = true) const override;
  void reset();

  // OPERATORS -----------------------------------------------------------------
  Point& operator=(Point const &other);
  Point operator+(Point const  &other) const;

  // METHODS -------------------------------------------------------------------
  // Calculate the projections: [1 1 0] and [2 1 0] -> [1 0 0]
  Point delta(Point const &other);
  // inherits from prev point: [1 - -] and [2 1 3] -> [1 1 3]
  void modal(Point const &other);
  data_t length() const;
  bool is_complete() const { return _x && _y && _z; }

  // ACCESSORS -----------------------------------------------------------------
  vector<data_t> vec() const;
  data_t x() const { return _x.value(); }
  data_t y() const { return _y.value(); }
  data_t z() const { return _z.value(); }

  data_t x(data_t v) { return (_x = v).value(); }
  data_t y(data_t v) { return (_y = v).value(); }
  data_t z(data_t v) { return (_z = v).value(); }
  
private:
  opt_data_t _x = nullopt;
  opt_data_t _y = nullopt;
  opt_data_t _z = nullopt;

}; // class Point

} // namespace cncpp




#endif // POINT_HPP