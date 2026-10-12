#include "units.hpp" // deg_to_rad, rpm_to_rad_s

#include <iostream> // cout, endl


int main() {
    std::cout << "90 deg = " << deg_to_rad(90) << " rad" << std::endl;

    double rad_s = rpm_to_rad_s(6000.0);

    return 0;
}