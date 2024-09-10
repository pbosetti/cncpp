/*
  ____       _       _          _
 |  _ \ ___ (_)_ __ | |_    ___| | __ _ ___ ___
 | |_) / _ \| | '_ \| __|  / __| |/ _` / __/ __|
 |  __/ (_) | | | | | |_  | (__| | (_| \__ \__ \
 |_|   \___/|_|_| |_|\__|  \___|_|\__,_|___/___/

*/

#ifndef POINT_HPP
#define POINT_HPP

#include "cncpp.hpp"
#include <optional>
#include <string>
#include <vector>

using namespace std;

namespace cncpp {

class Point : Object{
public:
  Point(opt_data_t x = nullopt, opt_data_t y = nullopt, opt_data_t z = nullopt);

  void modal(Point p);
  Point delta(Point p);
  data_t length();
  string desc(bool colored = true) override;
  bool complete() { return _x.has_value() && _y.has_value() && _z.has_value(); }
  vector<data_t> vec();

private:
  optional<data_t> _x = nullopt;
  optional<data_t> _y = nullopt;
  optional<data_t> _z = nullopt;
};

} // namespace cncpp

#endif // POINT_HPP