#pragma once

// For the comments on constructors, setters and getters
// look lesson_2026_10_13/src/thermostat.hpp

#include <ostream> // std::ostream, needed by operator<<
#include <string>

class Thermostat {
public:
  Thermostat() = default;
  Thermostat(std::string name, double setpoint, double hysteresis = 1.5);

  // Update the thermostat state. Every time the state changes
  // (ON -> OFF or OFF -> ON) the switch counter is incremented
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

  // Call operator: it makes the object callable like a function,
  // th(18.0) is the same as th.operator()(18.0).
  // Here it updates the state and returns the new one, so the
  // result depends also on the previous calls (a function with memory)
  bool operator()(double temperature);

  // Stream operator: it lets us write std::cout << th.
  // It is NOT a member function, since the left operand is the stream
  // and not the thermostat. Being "friend" it can still read the private
  // members of the class.
  // If we remove "friend" the compiler treats it as a member function, so
  // "this" is already the left operand and it complains that operator<<
  // has too many parameters.
  friend std::ostream &operator<<(std::ostream &out, const Thermostat &t);

private:
  std::string _name;
  double _setpoint = 20.0;
  double _hysteresis = 0.5;
  bool _is_heating = false;
  // How many times the thermostat switched ON/OFF
  int _switch_count = 0;
};
