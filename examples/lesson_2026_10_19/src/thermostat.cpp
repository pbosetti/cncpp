#include "thermostat.hpp"
#include <iostream>

Thermostat::Thermostat(std::string name, double setpoint, double hysteresis) {
  set_name(name);
  set_setpoint(setpoint);
  set_hysteresis(hysteresis);
}

void Thermostat::update(double temperature) {
  bool was_heating = _is_heating;

  if (temperature < _setpoint - _hysteresis) {
    _is_heating = true;
  } else if (temperature > _setpoint + _hysteresis) {
    _is_heating = false;
  }
  // Counting the number of times the state changed
  if (was_heating != _is_heating) {
    _switch_count++;
  }
}

void Thermostat::print() const {
  std::cout << "Thermostat " << _name << ": " << (_is_heating ? "ON" : "OFF")
            << " (setpoint " << _setpoint << " +/- " << _hysteresis << ", "
            << _switch_count << " switches)" << std::endl;
}

bool Thermostat::set_setpoint(double temperature) {
  // Same checks of last lesson, written with a single if
  if (temperature < 15.0 || temperature > 40.0) {
    return false;
  }
  _setpoint = temperature;
  return true;
}

bool Thermostat::set_hysteresis(double hysteresis) {
  if (hysteresis < 0.0) {
    return false;
  }
  _hysteresis = hysteresis;
  return true;
}

bool Thermostat::set_name(std::string name) {
  if (name.empty()) {
    return false;
  }
  _name = name;
  return true;
}

// It is a member function, so it has "Thermostat::" in front.
// We reuse update() instead of writing the same logic twice
bool Thermostat::operator()(double temperature) {
  update(temperature);
  return _is_heating;
}

// It is NOT a member function, so no "Thermostat::" in front.
// We write on "out" and not on std::cout, this way the same operator
// works with std::cout, std::cerr and files (std::ofstream).
// We do not put std::endl here: whoever uses the operator decides
// if and when to go to a new line.
std::ostream &operator<<(std::ostream &out, const Thermostat &t) {
  out << "Thermostat " << t._name << ": " << (t._is_heating ? "ON" : "OFF")
      << " (setpoint " << t._setpoint << " +/- " << t._hysteresis << ", "
      << t._switch_count << " switches)";
  // Returning the stream is what allows chaining:
  // std::cout << a << b is (std::cout << a) << b
  return out;
}
