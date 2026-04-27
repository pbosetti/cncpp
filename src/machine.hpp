#pragma once

#include "defines.hpp"
#include "point.hpp"
#include <nlohmann/json.hpp>

namespace cncpp {

class Machine : public Object {
  public:
  // LIFECYCLE =================================================================
  Machine() = default;
  Machine(nlohmann::json &j);
  Machine(std::string &filename);
  ~Machine() = default; 
  std::string desc(bool colored = true) const override;
  
  // OPERATIONS/OPERATORS ======================================================
  void load(nlohmann::json &j);
  void load(std::string &filename);
  data_t quantize(data_t t, data_t &dq) const;


  // ACCESSORS =================================================================
  data_t A() const { return _A; }
  data_t tq() const { return _tq; }
  data_t fmax() const { return _fmax; }
  data_t error() const { return _error; }
  data_t max_error() const { return _max_error; }
  Point zero() const { return _zero; }
  Point offset() const { return _offset; }
  Point setpoint() const { return _setpoint; }
  Point setpoint(Point p) { return _setpoint = p; }
  Point setpoint(data_t x, data_t y, data_t z) { return _setpoint = Point(x, y, z); } 
  Point position() const { return _position; }
  Point position(Point p) { return _position = p; }

  private:
  Point _zero{0, 0, 0};
  Point _offset{0, 0, 0};
  Point _setpoint, _position;
  data_t _tq = 0.001;        // 1 ms
  data_t _A = 5.0;           // m^s/s
  data_t _fmax = 10000.0;    // mm/min
  data_t _max_error = 0.005; // mm
  data_t _error = 0.0;       // mm
};

} // namespace cncpp