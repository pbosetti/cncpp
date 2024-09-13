/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
Machine class header file
Author: Paolo Bosetti, 2024
*/
#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "defines.hpp"
#include "point.hpp"
#include <string>

using namespace std;

namespace cncpp {

class Machine : Object {
public:
  // Lifecycle
  Machine(const std::string &ini_file);
  Machine() {}
  ~Machine();

  // Methods
  void load(const string &ini_file);
  string desc(bool colored = true) const override;
  data_t quantize(data_t t, data_t &dq) const;

  // Accessors
  Point position() const { return _position; }
  Point setpoint() const { return _setpoint; }
  Point offset() const { return _offset; }
  Point zero() const { return _zero; }
  data_t max_error() const { return _max_error; }
  data_t error() const { return _error; }
  data_t fmax() const { return _fmax; }
  data_t tq() const { return _tq; }
  data_t A() const { return _A; }

private:
  std::string _ini_file;
  data_t _A;                     // Maximum acceleration (m/s/s)
  data_t _tq;                    // Sampling time (s)
  data_t _max_error, _error;      // Maximum and actual positioning error (mm)
  data_t _fmax;                  // Maximum feedrate (mm/min)
  Point _zero;                   // Initial machine position
  Point _setpoint, _position;     // Setpoint and actual position
  Point _offset;                 // Workpiece origin coordinates
  bool _initialized = false;
};


}





#endif // MACHINE_HPP