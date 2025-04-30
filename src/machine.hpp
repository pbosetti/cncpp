/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
*/

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "defines.hpp"
#include "point.hpp"

using namespace std;

namespace cncpp {

class Machine final : Object {
public:
  // Lifecycle -----------------------------------------------------------------
  Machine(const string &settings_file);
  Machine() {}
  ~Machine() {};

  // Methods -------------------------------------------------------------------
  void load(const string &settings_file);
  string desc(bool colored = true) const override;
  data_t quantize(data_t t, data_t &dq) const;

  // Accessors -----------------------------------------------------------------
  data_t A() const { return _A; }
  data_t tq() const { return _tq; }
  data_t fmax() const { return _fmax; }
  data_t error() const { return _error; }
  data_t max_error() const { return _max_error; }

  Point zero() const { return _zero; }
  Point offset() const { return _offset; }

private:
  // parameters
  string _settings_file = "";
  Point _zero = Point(0, 0, 0);
  Point _offset = Point(0, 0, 0);
  data_t _A = 5.0; // m/s/s
  data_t _tq = 0.005; // sampling time (s)
  data_t _fmax = 10000;
  data_t _max_error = 0.005;

  // State variables
  data_t _error = 0.0;
};


}





#endif // MACHINE_HPP