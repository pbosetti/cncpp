#include "forced_spring.hpp"
#include "spring.hpp"

#include <cmath>
#include <fmt/format.h>
#include <iostream>
#include <rang.hpp>

int main() {
  std::cout << fmt::format("CUSTOM SIMULINK") << std::endl;
  std::cout << fmt::format("{:^40}", "CUSTOM SIMULINK") << std::endl;
  std::cout << fmt::format("{:^40}", "v1.0") << std::endl;
  std::cout << rang::bg::green << "Starting simulation" << rang::bg::reset
            << std::endl;

  Spring spring("spring");
  ForcedSpring fspring_1("constant_force");
  ForcedSpring fspring_2("sinusoidal_force");

  spring.start_log("./out/");
  fspring_1.start_log("./out/");
  fspring_2.start_log("./out/");

  spring.set_x0({1.0, 0.0});
  fspring_1.set_x0({0.0, 0.0});
  fspring_2.set_x0({0.0, 0.0});

  double tf = 20.0;
  double dt = 0.001;
  for (double t = 0.0; t < tf; t += dt) {
    spring.step(dt, {});
    fspring_1.step(dt, {1.0});
    fspring_2.step(dt, {1.0 + std::sin(t * 2 * M_PI * 10.0)});

    spring.csv_row(std::cout);
  }

  return 0;
}
