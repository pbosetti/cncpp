#include "thermostat.hpp"

#include <iostream>

int main() {
    Thermostat th_1("A212", 21.0, 0.5);
    th_1.print();

    // We cannot do this since the member variables are private
    // th_1._name = "A110";
    // th_1._is_heating = false;

    // Setting the new setpoint and checking if it passed
    // the internal logics checks
    if (th_1.set_setpoint(45.0) == false) {
        std::cerr << "failed setting setpoint" << std::endl;
    }
    // Setting and getting the new setpoint
    th_1.set_setpoint(22.0);
    std::cout << "setpoint: " << th_1.setpoint() << std::endl;

    // Constant double. It cannot be modified
    const double pi_approx = 3.1415;
    // pi_approx = 3;

    // Constant thermostat. I can call
    // only methods that are marked as const
    const Thermostat th_2("A212", 21.0, 0.5);
    th_2.print();
    // th_2.update(20.0);

    return 0;
}