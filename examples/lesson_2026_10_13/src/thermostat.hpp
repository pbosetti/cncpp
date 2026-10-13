// Again first line should be "#pragma once"
#pragma once

// For other comments look lesson_2026_10_12/example_4

#include <string>

class Thermostat {
public:
    // Creating a default constructor so that we can create the object without
    // specifying the arguments.
    // The default value of the member variables will be either:
    // - default value defined by c++ (bool -> false, int/float/double = 0)
    // - the value setted in the member variable declaration below (last lines of the class)
    Thermostat() = default;

    // Constructor with arguments and one argument with a default value for the hysteresis
    // so that the user can use the default one (1.5) or use his own value.
    Thermostat(std::string name, double setpoint, double hysteresis = 1.5);

    // Update the thermostat state
    void update(double temperature);
    // Print the thermostat state,
    // now is CONST, meaning we know it will not change
    // any member variable
    void print() const;

    // Setters to set the value of the member variables
    // We use this to add some logics/checks on the user inputs
    bool set_setpoint(double temperature);
    bool set_hysteresis(double hysteresis);
    bool set_name(std::string name);

    // Getters to get the value of member variables.
    // These functions are const since will never modify
    // the member variables.
    double setpoint() const;
    // For short functions we can also implement the function
    // directly in the header. We can also write them
    // in one line.
    double hysteresis() const { return _hysteresis; }
    bool is_heating() const { return _is_heating; }
    std::string name() const { return _name; }

private:
    std::string _name;
    double _setpoint = 20.0;
    double _hysteresis = 0.5;
    bool _is_heating;
};