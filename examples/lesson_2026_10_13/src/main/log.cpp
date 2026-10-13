#include "thermostat.hpp"

#include <iostream> // cout,cerr
#include <fstream>  // ofstream
#include <random>   // mt19937, normal_distribution

int main() {
    // Creating a thermostat with default constructor
    Thermostat default_thermostat;
    // Creating a thermostat with default hysteresis argument
    Thermostat room_2("A212", 21.0);
    
    // Create two thermostat with and without hysteresis
    Thermostat room("A212", 21.0, 0.5);
    Thermostat no_band("A212", 21.0, 0.0);

    // Variables that define the room dynamics
    double true_temp = 18.0;
    const double heating_rate = 0.1;
    const double cooling_rate = 0.05;
    const double dt = 10.0;
    const int steps = 360;

    // Random number generator
    std::mt19937 gen(42);
    // Normal distribution with mean=0.0 and standard deviation of 0.2
    std::normal_distribution<double> noise(0.0, 0.2);

    // Defining a string for the csv path
    std::string path = "log.csv";
    // Opening the file. std::ofstream means 
    // output file stream, meaning a output file.
    // use std::ifstream if you want to read from file.
    std::ofstream file(path);
    // Check wether the file was opened.
    if (!file.is_open()) { // same as if (file.is_open() == false){}
        std::cerr << "failed opening file" << std::endl;
        return 1;
    }

    // Do not do for loops like this, the division error is accumulated
    // and after 30 iterations the time is not 10.0 seconds!!!!
    // for (double t = 0.0; t < 100.0; t += 1.0/3.0) {}

    // Writing the csv header
    file << "time_s,true_temp,measured,heating,heating_no_band" << std::endl;
    for (int step = 0; step < steps; step ++) {
        double t = step * dt;
        
        // Simulating a sensor which is reading
        // the true temperature but with gaussian white noise
        double measured = true_temp;
        measured += noise(gen);

        // Update the thermostats states.
        // Here we have to use the measured temperature
        room.update(measured);
        no_band.update(measured);

        // Use room thermostat to heat the room
        if (room.is_heating()) {
            true_temp += heating_rate;
        } else {
            true_temp -= cooling_rate;
        }

        // Write the CSV row
        file << t << ","
            << true_temp << ","
            << measured << ","
            << room.is_heating() << ","
            << no_band.is_heating() << std::endl;
    }
    // Closing the CSV file
    file.close();

    return 0;
}