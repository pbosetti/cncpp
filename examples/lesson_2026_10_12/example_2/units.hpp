// This line (#pragma once) should elways be at the top of
// the header files.
#pragma once

// Declaring a function named deg_to_rad which will return
// a double value and gets in input a double value.
double deg_to_rad(double deg);
double rpm_to_rad_s(double rpm);
// The lines above just tell that this function will be
// implemented somewhere, as a convention, it will be
// implemented in the source file with the same name,
// in this case units.cpp
// Without implementing the function the compilation fails
// with a linking error: something like "undefined symbol"