// This line includes the file "units.hpp".
// It is literally copying the content here.
// This will allow us to use its content (types,
// functions etc...)
#include "units.hpp"

// Including a system library cmath, we need
// this lib to use M_PI.
#include <cmath>

// These are the function implementations
double deg_to_rad(double deg) {
    return deg * M_PI / 180.0;
}
double rpm_to_rad_s(double rpm) {
    return rpm / 60.0 * (2 * M_PI);
}
