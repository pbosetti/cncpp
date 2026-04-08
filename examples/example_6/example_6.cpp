#include "forced_spring.hpp"
#include "spring.hpp"

#include <cmath>
#include <fmt/format.h>
#include <iostream>
#include <rang.hpp>

int main() {
  // I can use fmt to format a string
  std::cout << fmt::format("CUSTOM MODELS") << std::endl;
  // Here I am chosing to format the string to be 40 characters wide
  // and to center (with ^) the string "CUSTOM MODELS".
  std::cout << fmt::format("{:^40}", "CUSTOM MODELS") << std::endl;
  // Here another centered string
  std::cout << fmt::format("{:^40}", "v1.0") << std::endl;

  // With the rang library we can change the background color (with bg), or the
  // foreground color (with fg) or even the style of the text (italic/bold)
  std::cout << rang::bg::green << "Starting simulation" << rang::bg::reset
            << std::endl;

  // Instantiate our models
  Spring spring("spring");
  ForcedSpring fspring_1("constant_force");
  ForcedSpring fspring_2("sinusoidal_force");

  // Start the log in the "./out/" folder
  spring.start_log("./out/");
  fspring_1.start_log("./out/");
  fspring_2.start_log("./out/");

  // Set their initial conditions
  spring.set_x0({1.0, 0.0});
  fspring_1.set_x0({0.0, 0.0});
  fspring_2.set_x0({0.0, 0.0});

  // Performs the actual simulation.
  // Too large integration steps will generate drifts in the solution.
  double tf = 20.0;
  double dt = 0.001;
  for (double t = 0.0; t < tf; t += dt) {
    // No inputs
    spring.step(dt, {});
    // One constant force
    fspring_1.step(dt, {1.0});
    // Constant force + sinusoid
    fspring_2.step(dt, {1.0 + std::sin(t * 2 * M_PI * 10.0)});

    // Using the same csv_row function to print to the terminal instead of
    // a file.
    spring.csv_row(std::cout);
  }

  return 0;
}
