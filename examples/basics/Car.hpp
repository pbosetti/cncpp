#ifndef INCLUDE_CAR_HPP
#define INCLUDE_CAR_HPP

#include <iostream>

class Car {
public:
  Car(std::string name, std::string brand) : _name(name), _brand("Unknown")
  {
      std::cerr << "Car initialized" << std::endl;
  }
  virtual ~Car() {
      std::cout << "Car destroyed" << std::endl;
  }

  void start() { _is_started = true; }
  void stop() { _is_started = false; }

  void say_hello();

  // accessors:
  std::string name() const { return _name; }
  std::string brand() const { return _brand; }

private:
  std::string _name;
  std::string _brand;
  bool _is_started;
};

#endif // INCLUDE_CAR_HPP