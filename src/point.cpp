/*
  ____       _       _          _               
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___ 
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/
                                                
*/

#include "point.hpp"
#include <exception>
#include <sstream>

using namespace std;
using namespace cncpp;

Point::Point(opt_data_t x, opt_data_t y, opt_data_t z) : _x(x), _y(y), _z(z) {}

// Set all coords to nullopt (undefined)
void Point::reset() {
  _x.reset();
  _y.reset();
  _z.reset();
}

void Point::modal(const Point &p) {
  if (p._x && ! _x)
    _x = p._x;
  if (p._y && ! _y)
    _y = p._y;
  if (p._z && ! _z)
    _z = p._z;
}

Point Point::operator+(const Point &other) const {
  if (!is_complete() || !other.is_complete())
    throw runtime_error("Points are not complete");
  
  Point out(
    _x.value() + other._x.value(),
    _y.value() + other._y.value(),
    _z.value() + other._z.value()
  );
  return out;
}

Point Point::delta(const Point &other) {
  if (!is_complete() || !other.is_complete())
    throw runtime_error("Points are not complete");

  Point out(
    _x.value() - other._x.value(),
    _y.value() - other._y.value(),
    _z.value() - other._z.value()
  );
  return out;
}

data_t Point::length() const {
  if (!is_complete())
    throw runtime_error("Points are not complete");

  return sqrt(
    _x.value() * _x.value() +
    _y.value() * _y.value() +
    _z.value() *_z.value()
  );
}

// descrition like: [100.0, 200.0, 123.2]
string Point::desc() const {
  stringstream ss;
  ss << "["
     << _x.value() << ", "
     << _y.value() << ", "
     << _z.value() << "]";

  return ss.str();
}



/*
  _____         _   
 |_   _|__  ___| |_ 
   | |/ _ \/ __| __|
   | |  __/\__ \ |_ 
   |_|\___||___/\__|
                    
*/

#ifdef POINT_MAIN


int main() {
  Point p1(1, 2, 3);
  return 0;
}



#endif //POINT_MAIN