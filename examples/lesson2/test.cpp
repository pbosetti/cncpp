#include <string> // to use std::string
#include <iostream> // to use std::cout
#include <vector> // to use std::vector

/*
CMAKE USAGE:
mkdir build
cd build
cmake ..
make -j8
*/

// Class to describe a vehicle
class Vehicle {
private: // cannot be seen externally from the class
  std::string name{"none"};
  std::string brand{"none"};
  int wheels_number{4}; // numer of wheel are 4 by default
  
public: // everyone can see and iteract through what is stated below
  Vehicle() {} // Default class constructor

  Vehicle(std::string b, std::string n) 
    : name(n), brand(b) // Minor improvement
  {}

  Vehicle(std::string b, std::string n, int w) {
    this->name = n;
    this->brand = b;
    this->wheels_number = w;
  }

  // fuction to print the brand and the name
  void print(void) {
    std::cout << "I have a " << this->brand << " " << this->name << std::endl;
  }
};

// Car inherits from vehicle all its properties
class Car : public Vehicle {
public:
  Car(std::string b, std::string n) 
  : Vehicle(b, n, 4) // Call the parent constructor
  {/*add other code for further specialization*/} 
};

int main() {

  // Using Vehicle class
  Vehicle empty_vehicle;
  empty_vehicle.print();
  Vehicle car("bmw", "320");
  car.print();
  Vehicle bus("mercedes", "bla");
  bus.print();

  // Using Car class
  Car car1("renault", "clio");
  car1.print();

  // Usage of vector
  std::vector<int> v1;
  std::vector<double> v2;
  std::vector<std::string> v3;
  std::vector<Car> v4;

  // Specify the numer of elements
  std::vector<int> v(10); // vector of 10 integers
  if (v.size() == 10) { // access the current size
    v.resize(5); // shrink the size to 5
  }
  v.reserve(100); // this grants room for resizing to 100 elements

  // Accessing the elements of the vector
  v[0] = 1; // assigning the first element of our vector
  v[11] = 3; // does not give an error
  // v.at(11) = 3; //grants us the safety of accessing the elements

  // Push and poping elements
  std::vector<double> vd{1.0, 2.0};
  //vd.reserve(10); // reserve the memory for 3 elements
  vd.push_back(3.0);
  vd.push_back(4.0);
  vd.push_back(5.0);
  vd.push_back(6.0);
  vd.pop_back(); // removes the last element

  for (size_t i = 0; i < vd.size(); ++i)
  {
    std::cout << vd[i] << std::endl;
  }

  return 0;
}

// Exercise:
// Build a class that perform the Hadamard product (element-wise) between vector
// [1, 2, 3]*[1, 2, 3] = [1, 4, 9]
// The class must have a counter which counts the number of multiplications 
// between numbers
// Optionally, templetize it to handle different numeric types


