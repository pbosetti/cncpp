#include "thermostat.hpp"
#include <fstream>  // ofstream
#include <iostream> // cout, cerr
#include <vector>

int main() {
  // An operator is a function with a special name:
  // v[1] calls operator[], std::cout << x calls operator<<.
  // We already used the operators of the standard library
  std::vector<int> v = {1, 2, 3};
  std::cout << v[1] << std::endl; // operator[] of std::vector
  // Nobody wrote operator<< for std::vector, so this does not compile.
  // The errors are a lot, read the first one: "no match for operator<<"
  // std::cout << v << std::endl;

  Thermostat t("A212", 20.0, 0.5);
  t.update(22.0);
  // Calling the object like a function, same as t.update(18.0)
  // but it also returns the new state
  t(18.0);

  // The output is the same, but with operator<< we can choose
  // where to write and we can chain other things after it
  t.print();
  std::cout << t << std::endl;
  std::cerr << t << std::endl;
  std::cout << t << " | " << Thermostat("A101", 18.0, 1.0) << std::endl;

  // Same operator, but writing on a file
  std::ofstream fout("log.txt");
  if (!fout.is_open()) {
    std::cerr << "failed opening file" << std::endl;
    return 1;
  }
  fout << t << std::endl;
  fout.close();

  // operator() returns the state (1 = ON, 0 = OFF), and the result
  // depends also on what happened before (hysteresis):
  // with 20.3 we are inside the band, so the state does not change
  std::cout << "15.0 -> " << t(15.0) << std::endl; // ON
  std::cout << "20.3 -> " << t(20.3) << std::endl; // inside the band: ON
  std::cout << "21.0 -> " << t(21.0) << std::endl; // OFF
  std::cout << "20.3 -> " << t(20.3) << std::endl; // inside the band: OFF
  std::cout << t << std::endl;

  return 0;
}
