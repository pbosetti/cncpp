#include "thermostat.hpp"
#include <algorithm>  // std::sort, std::count_if, std::find_if
#include <cmath>      // std::pow
#include <functional> // std::function, std::greater
#include <iostream>
#include <list>
#include <vector>

// The print function can be written as a normal function...
// void print(const std::vector<int> &values) {
//   for (int v : values) {
//     std::cout << v << ",";
//   }
//   std::cout << std::endl;
// }

// std::function<double(double)> is the type for "anything callable that
// takes a double and returns a double": a function, a lambda, a function
// object... This is how our own functions can accept lambdas as arguments.
// Note: here the template parameter is a function signature
// (return_type(argument_types)).
// The CNC project passes callbacks in this way.
void apply_to_all(std::vector<double> &values,
                  const std::function<double(double)> &f) {
  // f is called like a normal function
  // double ret = f(10.0);
  for (double &v : values) { // & so that we modify the vector elements
    v = f(v);
  }
}

int main() {
  // ...or as a lambda stored in a local variable
  auto print = [](const std::vector<int> &values) {
    for (int v : values) {
      std::cout << v << ",";
    }
    std::cout << std::endl;
  };

  // <algorithm> gives ready-made loops working on a range [begin, end)
  std::vector<int> grades = {24, 18, 30, 27, 21};

  // 1. Sort. By default it uses operator< (ascending order)
  std::sort(grades.begin(), grades.end());
  print(grades);

  // Sort with a function object from the library: std::greater<int> is a
  // template AND a class with operator() that returns a > b (descending)
  std::sort(grades.begin(), grades.end(), std::greater<int>());
  print(grades);

  // Sort with a lambda: it receives two elements and returns true if
  // "a" must come before "b"
  std::sort(grades.begin(), grades.end(), [](int a, int b) { return a < b; });
  print(grades);

  // 2. Count the elements that satisfy a condition (given as a lambda)
  auto excellent = std::count_if(grades.begin(), grades.end(),
                                 [](int g) { return g > 27; });
  std::cout << "excellent: " << excellent << std::endl;

  // Find the first element that satisfies a condition. To use a local
  // variable inside the lambda we must capture it: [&] captures everything
  // by reference, [average] would capture only average by copy.
  int average = 26;
  auto it = std::find_if(grades.begin(), grades.end(),
                         [&](int g) { return g > average; });
  // find_if returns an iterator: end() means "not found", so check it
  // before dereferencing
  if (it != grades.end()) {
    int found_grade = *it;
    std::cout << "first element: " << found_grade << std::endl;
  }

  // 3. Our own objects can be sorted by any criterion: here by setpoint.
  // emplace_back constructs the Thermostat directly inside the vector,
  // passing the arguments to its constructor.
  std::vector<Thermostat> rooms;
  rooms.emplace_back("B107", 23.0, 0.5);
  rooms.emplace_back("A110", 22.0, 0.5);
  rooms.emplace_back("A212", 21.0, 0.5);
  std::sort(rooms.begin(), rooms.end(),
            [](const Thermostat &a, const Thermostat &b) {
              return a.setpoint() < b.setpoint();
            });
  // Printing works thanks to the operator<< we wrote for Thermostat
  for (const auto &t : rooms) {
    std::cout << t << std::endl;
  }

  // 4. std::sort needs to jump around the container, so it does not work
  // on a list. A list has its own .sort() method.
  // If you uncomment the ERROR line, the error is reported INSIDE the
  // library: scroll up to "required from here" to find your line.
  std::list<int> numbers = {5, 3, 8, 1};
  // std::sort(numbers.begin(), numbers.end()); // ERROR: needs a vector
  numbers.sort();
  std::cout << "list front after sort: " << numbers.front() << std::endl;

  // 5. Passing lambdas to our own functions (see apply_to_all above).
  // "-> double" explicitly declares the return type of the lambda
  // (optional here, the compiler can deduce it).
  std::vector<double> prices = {1.20, 3.50, 4.30};
  apply_to_all(prices, [](double price) -> double {
    return price * (1.0 + 0.22); // add the VAT
  });
  for (const auto &p : prices) {
    std::cout << p << std::endl;
  }
  // The same function, with a different behaviour: square each price
  apply_to_all(prices,
               [](double price) -> double { return std::pow(price, 2.0); });

  return 0;
}
