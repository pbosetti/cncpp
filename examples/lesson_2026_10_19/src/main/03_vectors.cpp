#include "thermostat.hpp"
#include <iostream>
#include <vector>

// Vectors (and objects in general) are passed by const reference:
// copying a million doubles just to compute the average is a waste
double average(const std::vector<double> &values) {
  double sum = 0.0;
  for (double v : values) {
    sum += v;
  }
  return sum / values.size();
}

int main() {
  // Creating a vector with 3 elements and adding one at the end.
  // The vector grows by itself when needed
  std::vector<double> temps = {19.5, 20.0, 21.5};
  temps.push_back(22.0);
  std::cout << "size: " << temps.size() << std::endl;

  // Accessing the elements
  std::cout << "first: " << temps[0] << " second: " << temps.at(1)
            << " last: " << temps.back() << std::endl;
  // [] does NOT check the index: we read garbage or the program crashes
  // std::cout << temps[10] << std::endl;
  // at() checks the index and stops the program with an out_of_range error
  // std::cout << temps.at(10) << std::endl;

  // Loop with the index. The index is size_t (unsigned integer), using
  // int gives a warning since size() returns a size_t
  for (size_t i = 0; i < temps.size(); i++) {
    std::cout << i << " -> " << temps[i] << std::endl;
  }
  // Range-for loop with a reference: we modify the elements of the vector
  for (double &t : temps) {
    t += 1.0;
  }
  for (size_t i = 0; i < temps.size(); i++) {
    std::cout << i << " -> " << temps[i] << std::endl;
  }
  std::cout << "average: " << average(temps) << std::endl;

  // Vector of our own class
  std::vector<Thermostat> rooms;
  // push_back COPIES the new thermostat into the vector
  rooms.push_back(Thermostat("lab", 21.0, 0.5));
  // emplace_back builds the thermostat directly inside the vector,
  // we pass the arguments of the constructor
  rooms.emplace_back("A212", 22.0, 0.5);

  // Same three choices of 01_references.cpp:
  // Thermostat t -> copy, the update is lost
  for (Thermostat t : rooms) {
    t.update(17.0);
  }
  // Thermostat &t -> original, the update stays
  for (Thermostat &t : rooms) {
    t.update(17.0);
  }
  // const Thermostat &t -> original, only reading
  for (const Thermostat &t : rooms) {
    t.print();
  }

  // Removing the last element and then all the elements
  rooms.pop_back();
  std::cout << "size after pop_back: " << rooms.size() << std::endl;
  rooms.clear();
  std::cout << "size after clear: " << rooms.size() << std::endl;
  std::cout << "empty: " << rooms.empty() << std::endl;

  // reserve() only allocates the memory: the size is still 0, there are
  // no elements inside! It is useful when we know how many elements we
  // are going to add, so that the vector does not have to grow many times
  std::vector<Thermostat> rooms_reserved;
  rooms_reserved.reserve(10000);
  std::cout << "reserve -> size: " << rooms_reserved.size()
            << " capacity: " << rooms_reserved.capacity() << std::endl;
  // rooms_reserved.at(1); // Error: there is no element 1
  for (size_t i = 0; i < 10000; i++) {
    rooms_reserved.push_back(Thermostat());
  }

  // resize() instead creates the elements, all copies of the
  // thermostat that we pass as second argument
  std::vector<Thermostat> rooms_resized;
  rooms_resized.resize(10000, Thermostat("lab", 21.0, 0.5));
  std::cout << "resize -> size: " << rooms_resized.size() << std::endl;
  Thermostat s = rooms_resized.at(1); // Valid
  s.print();

  return 0;
}
