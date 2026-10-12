// Again first line should be "#pragma once"
#pragma once

// Including string so that we can use
// std::string _name;
#include <string>

// Creating a class called Thermostat
class Thermostat {

// everything after this will be public, aka visible
// from outside this class, more on that on the next lesson
public:

// Declaring the constructor of this class with 3 arguments.
// It is similar to a function declaration (ex "void describe(int value);"),
// but it does not have the return type (because will be the class Thermostat)
// And the function name MUST be the class name, in this case "Thermostat"
    Thermostat(std::string name, double setpoint, double hysteresys);

    // Methods of the class
    void update(double temperature);
    void print();

    // Class attributes
    // aka class member variables.
    // As convention we will prepend the variable name
    // with an underscore to distinguish between a
    // member variable and a normal variable
    std::string _name;
    double _setpoint;
    double _hysteresys;
    bool _is_heating;
};