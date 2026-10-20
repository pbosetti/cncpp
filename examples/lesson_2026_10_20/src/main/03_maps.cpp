#include "thermostat.hpp"
#include <cmath>    // std::sqrt
#include <iostream>
#include <map>      // std::map
#include <optional> // std::optional
#include <string>
#include <vector>

// std::optional<T> holds either a value of type T or NOTHING (std::nullopt).
// Useful for arguments that may be missing: e.g. a G-code line "G1 X10"
// gives only X, so Y must not move.
void cnc_move(std::optional<double> x, std::optional<double> y) {
  // Check if the optional contains a value...
  if (x.has_value()) {
    // move_axis(x.value());
  }
  // ...or, the short version: an optional converts to bool
  if (y) {
    // move_axis(y.value());
  }
}

// std::optional is also useful as a return value of a function that can
// fail: return the value on success, std::nullopt otherwise.
std::optional<double> c_sqrt(double value) {
  if (value > 0.0) {
    return std::sqrt(value);
  }
  return std::nullopt;
}

int main() {
  // Empty optionals (no value)
  std::optional<double> x;
  std::optional<double> y;
  // Move both axes to 0
  x = 0.0;
  y = 0.0;
  cnc_move(x, y);
  // Move only X, Y has no value
  x = 10.0;
  y = std::nullopt;
  cnc_move(x, y);

  // sqrt of a negative number fails: s is empty and nothing is printed
  std::optional<double> s = c_sqrt(-1.0);
  if (s) {
    std::cout << "valid sqrt" << s.value() << std::endl;
  }

  // 1. A map stores key -> value pairs, like a dictionary. It takes two
  // template parameters: the key type and the value type.
  // [] both reads and writes. Assigning to an existing key overwrites it.
  std::map<std::string, int> ages;
  ages["Mario"] = 20;
  ages["Anna"] = 22;
  ages["Luca"] = 21;
  std::cout << ages["Mario"] << " " << ages.size() << std::endl;

  // 2. PITFALL: [] on a missing key CREATES it, with value 0 (or the
  // default-constructed value). Here size() becomes 4!
  std::cout << ages["Marioo"] << " " << ages.size() << std::endl;
  // at() instead does not create the key: if missing it throws
  // std::out_of_range and stops the program
  // std::cout << ages.at("Mariooo") << " " << ages.size() << std::endl;

  // 3. Iteration is ALWAYS sorted by key (alphabetical for strings).
  // Each element is a std::pair: .first is the key, .second is the value
  // std::pair<std::string, int> p;
  for (const auto &p : ages) {
    std::cout << p.first << " " << p.second << std::endl;
  }
  // Same loop with structured bindings: we give names to first and second
  for (const auto &[name, age] : ages) {
    std::cout << name << " " << age << std::endl;
  }

  // 4. Safe lookups
  // std::cout << ages.at("Paolo") << std::endl; // stops with out_of_range
  // std::cout << ages["Paolo"] << std::endl;    // creates "Paolo" with 0
  // contains() checks if the key exists (C++20)
  if (ages.contains("Paolo")) {
    std::cout << ages.at("Paolo") << std::endl;
  }

  // find() returns an iterator to the element, or end() if not found
  auto it = ages.find("Paolo");
  if (it != ages.end()) {
    // (*it).first is the same as it->first
    std::cout << (*it).first << " " << (*it).second << std::endl;
  }

  // 5. Remove an element by key
  ages.erase("Marioo");

  // 6. Our own classes as values. [] needs the DEFAULT CONSTRUCTOR: if the
  // key is missing it first creates a default Thermostat, then assigns.
  // Try removing "Thermostat() = default;" from thermostat.hpp: a long list
  // of errors, and "required from here" points at the [] line.
  std::map<std::string, Thermostat> rooms;
  rooms["A110"] = Thermostat("A110", 21, 0.5);

  // Values can be containers too, e.g. the columns of a CSV file:
  // column name -> column values.
  // PITFALL: CSV["time"] creates an EMPTY vector, so [0] reads outside the
  // vector (undefined behaviour). Use push_back to add values, and .at(0)
  // for a checked access.
  std::map<std::string, std::vector<double>> CSV;
  CSV["time"][0];

  // std::unordered_map is used in the same way, but it is not sorted and
  // it is faster on big maps.
  // The CNC project uses a map in the state machine to look up names.

  return 0;
}
