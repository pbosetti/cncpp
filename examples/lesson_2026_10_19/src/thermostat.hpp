#pragma once

#include <string>

class Thermostat {
public:
  Thermostat() = default;
  Thermostat(std::string name, double setpoint, double hysteresis = 1.5);

  void update(double temperature);
  void print() const;

  bool set_setpoint(double temperature);
  bool set_hysteresis(double hysteresis);
  bool set_name(std::string name);

  std::string name() const { return _name; }
  double setpoint() const { return _setpoint; }
  double hysteresis() const { return _hysteresis; }
  bool is_heating() const { return _is_heating; }
  int switch_count() const { return _switch_count; }

private:
  std::string _name;
  double _setpoint = 20.0;
  double _hysteresis = 0.5;
  bool _is_heating = false;
  int _switch_count = 0;
};
