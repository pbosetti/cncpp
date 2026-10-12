#include "thermostat.hpp"

int main() {

    // Now we can use our class as a type!!
    // in this case we are constructing the object with 
    // some values for name, setpoint and hysteresys
    Thermostat th_1("A212", 21.0, 0.5);

    // Updating and printing the thermostat state
    th_1.update(10);
    th_1.print();
    th_1.update(30);
    th_1.print();
    th_1.update(20.8);
    th_1.print();

    // the for loop can be done with doubles too.
    for (double temp = 20.0; temp < 22.0; temp += 0.25) {
        th_1.update(temp);
        th_1.print();
    }

    return 0;
}