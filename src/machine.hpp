#pragma once

#include "defines.hpp"
#include "point.hpp"

namespace cncpp {

class Machine {
  public:
  cncpp::Point zero() const { return cncpp::Point(0,0,0); }
  data_t tq() const { return _tq; }
  data_t A() const { return 1000; }
  data_t error() const { return 1e-3; }
  data_t quantize(data_t t, data_t &dq) const {
    data_t q;
    q = static_cast<size_t>((t / _tq) + 1) * _tq;
    dq = q - t;
    return q;
  }

  private:
  data_t _tq = 0.001; // 1 ms
};

} // namespace cncpp