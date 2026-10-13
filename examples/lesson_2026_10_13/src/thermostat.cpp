#include "thermostat.hpp"
#include <iostream>

// This constructor here will initialize the arguments in a different way, 
// the result will be the same
// Thermostat::Thermostat(std::string name, double setpoint, double hysteresis)
//     : _name(name), _setpoint(setpoint), _hysteresis(hysteresis), _is_heating(false) {
// }

Thermostat::Thermostat(std::string name, double setpoint, double hysteresis) {
    // Instead of this:
    // _name = name;
    // _setpoint = setpoint;
    // _hysteresis = hysteresis;
    // Now we can do:
    set_name(name);
    set_setpoint(setpoint);
    set_hysteresis(hysteresis);
    _is_heating = false;
}

void Thermostat::update(double temperature) {
    bool was_heating = _is_heating;

    if (temperature < _setpoint - _hysteresis) {
        _is_heating = true;
    } else if (temperature > _setpoint + _hysteresis) {
        _is_heating = false;
    }
    if (was_heating != _is_heating) {
        // std::cout << "Changed state" << std::endl;
    }
}

void Thermostat::print() const {
    std::cout << "Thermostat:" << std::endl;
    std::cout << "\tstate: " << (_is_heating ? "ON" : "OFF") << std::endl;

    std::cout << "\tstate: ";
    if(_is_heating) {
        std::cout << "ON";
    } else {
        std::cout << "OFF";
    }
    std::cout << std::endl;
}

bool Thermostat::set_setpoint(double temperature) {
    if (temperature < 15.0) {
        return false;
    }
    if (temperature > 40.0) {
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

double Thermostat::setpoint() const {
    return _setpoint;
}