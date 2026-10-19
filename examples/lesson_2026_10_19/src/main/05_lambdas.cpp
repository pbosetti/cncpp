#include "thermostat.hpp"
#include <iostream>
#include <string>
#include <utility> // std::pair
#include <vector>

// A normal function, to compare it with the lambda below
double square_func(double x) {
  return x * x;
}

int main() {
  // auto: the compiler writes the type for us, deducing it
  // from the value on the right
  int a = 3;
  auto b = a; // b is an int

  // auto is useful when the type is very long, like this one
  std::vector<std::pair<std::string, std::vector<Thermostat>>> buildings;
  buildings.push_back({"povo", {Thermostat("A212", 21.0, 0.5)}});
  // These three loops do the same thing, but the first two copy
  // each element (the string and the whole vector of thermostats!)
  for (std::pair<std::string, std::vector<Thermostat>> building : buildings) {
    std::cout << building.first << std::endl;
  }
  for (auto building : buildings) {
    std::cout << building.first << std::endl;
  }
  // As always, if we only read: const reference
  for (const auto &building : buildings) {
    std::cout << building.first << std::endl;
  }

  // The type is decided once, when the variable is created
  double c = 7.9;
  b = c; // b is still an int, so it becomes 7
  std::cout << "b = " << b << std::endl;

  // A lambda is a function without a name: [capture](parameters) { body }
  // We store it in an auto variable, since its type has no name we can write.
  // The return type is deduced, but we can force it with -> double
  auto square = [](double x) -> double { return x * x; };
  std::cout << square_func(10.0) << " " << square(10.0) << std::endl;

  return 0;
}
