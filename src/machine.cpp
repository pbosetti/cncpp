#include "machine.hpp"
#include <toml++/toml.h>
#include <sstream>


namespace cncpp {

Machine::Machine(const std::string &ini_file) : _ini_file(ini_file) {
  load(_ini_file);
}

Machine::~Machine() {}

void Machine::load(const string &ini_file) {
  _ini_file = ini_file;
  auto data = toml::parse_file(ini_file);
  auto machine = data["machine"];
  _A = machine["A"].value_or(10.0);
  _tq = machine["tq"].value_or(0.005);
  _max_error = machine["max_error"].value_or(0.001);
  _fmax = machine["fmax"].value_or(10000.0);
  _zero = Point(
    machine["zero"][0].value_or(0.0), 
    machine["zero"][1].value_or(0.0), 
    machine["zero"][2].value_or(0.0)
  );
  _offset = Point(
    machine["offset"][0].value_or(0.0), 
    machine["offset"][1].value_or(0.0), 
    machine["offset"][2].value_or(500.0)
  );
  _initialized = true;
}

string Machine::desc(bool colored) const {
  if (!_initialized) throw CNCError("Not initialized", this);
  ostringstream ss;
  ss << "Machine: ";
  ss << "A=" << _A << ", ";
  ss << "tq=" << _tq << ", ";
  ss << "max_error=" << _max_error << ", ";
  ss << "fmax=" << _fmax << ", ";
  ss << "zero=" << _zero.desc(false) << ", ";
  ss << "offset=" << _offset.desc(false);
  return ss.str();
};

data_t Machine::quantize(data_t t, data_t &dq) const {
  if (!_initialized) throw CNCError("Not initialized", this);
  data_t q;
  q = ((size_t)(t / _tq) + 1) * _tq;
  dq = q - t;
  return q;
}


}
