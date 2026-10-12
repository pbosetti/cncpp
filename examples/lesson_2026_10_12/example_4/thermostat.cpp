#include "thermostat.hpp"
#include <iostream>

// This constructor here will initialize the arguments in a different way, 
// the result will be the same
// Thermostat::Thermostat(std::string name, double setpoint, double hysteresys)
//     : _name(name), _setpoint(setpoint), _hysteresys(hysteresys), _is_heating(false) {
// }

// Constructor implementation. again no return value.
// to distinguish from a normal function, we have to prepend by the class name::
Thermostat::Thermostat(std::string name, double setpoint, double hysteresys) {
    // initializing the member variables
    _name = name;
    _setpoint = setpoint;
    _hysteresys = hysteresys;
    _is_heating = false;
}

// Core logic of the thermostat
void Thermostat::update(double temperature) {
    // copying the current value of the
    // _is_heating member variable
    bool was_heating = _is_heating;

    // Do nothing if the temperature is 
    // withing the saetpoint +- hysteresys.

    if (temperature < _setpoint - _hysteresys) {
        _is_heating = true;
    } else if (temperature > _setpoint + _hysteresys) {
        _is_heating = false;
    }

    // Checking if the state changed
    if (was_heating != _is_heating) {
        std::cout << "Changed state" << std::endl;
    }
}

void Thermostat::print() {
    std::cout << "Thermostat:" << std::endl;
    // the one below is a inline if. It is used to write short conditions like in this case.
    // without the inline if the code whould be longer (like below).
    // condition ? code_if_true : code_if_false;
    std::cout << "\tstate: " << (_is_heating ? "ON" : "OFF") << std::endl;

    // the following lines will do the same as the line above!  
    std::cout << "\tstate: ";
    if(_is_heating) {
        std::cout << "ON";
    } else {
        std::cout << "OFF";
    }
    std::cout << std::endl;
}