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
