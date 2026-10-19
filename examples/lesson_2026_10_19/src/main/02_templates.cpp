#include <algorithm> // std::max
#include <iostream>
#include <string>
#include <utility> // std::pair
#include <vector>

// This is (more or less) how std::max is declared in <algorithm>.
// T is a placeholder for "a type, chosen by whoever uses the function".
// The compiler writes a version of max for each type we actually use.
// template <class T>
// const T& max(const T& a, const T& b);

// Two ways of "returning" two values from a function.
// 1. Using references as output arguments
void measure_lab_by_ref(std::string &room, double &value) {
  room = "lab";
  value = 18.0;
}
// 2. Returning a std::pair, which holds two values of (possibly)
// different types
std::pair<std::string, double> measure_lab() {
  return {"lab", 18.0};
}

int main() {
  // The type T is deduced from the arguments
  std::cout << "Int: " << std::max(3, 7) << std::endl;        // max<int>
  std::cout << "Double: " << std::max(3.0, 7.0) << std::endl; // max<double>
  // Here the compiler cannot choose between int and double
  // std::cout << std::max(3, 7.1) << std::endl;
  // so we have to write the type between < >
  std::cout << "Double: " << std::max<double>(3, 7.1) << std::endl;
  // static_cast<int> converts the double to int, cutting the decimals,
  // so 7.9 becomes 7
  std::cout << "Int: " << std::max<int>(3, static_cast<int>(7.9)) << std::endl;

  // Containers are templates too: the type of the elements goes between < >
  // With { } the compiler does not allow converting a double to an int
  // (we could lose data), so we have to do it explicitly with static_cast
  double zero_count = 0.0;
  std::vector<int> counts = {static_cast<int>(zero_count), 1, 2, 3};
  std::vector<double> temps = {20.0, 21.0};
  std::vector<std::string> names = {"lab", "A212"};
  // This vector holds only int, a string cannot go inside
  // counts.push_back("hello");
  std::cout << counts.size() << " " << temps.size() << " " << names.size()
            << std::endl;

  // Templates can have more than one type, e.g. std::pair
  std::pair<std::string, double> reading = {"lab", 18.0};
  std::cout << reading.first << ": " << reading.second << std::endl;

  // Same result, with the two versions of measure_lab.
  // With the pair everything is in one line, and we see from the
  // signature of the function what it returns
  std::pair<std::string, double> measure = measure_lab();
  std::cout << measure.first << ": " << measure.second << std::endl;

  std::string label;
  double value;
  measure_lab_by_ref(label, value);
  std::cout << label << ": " << value << std::endl;

  return 0;
}
