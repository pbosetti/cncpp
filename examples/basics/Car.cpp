#include "Car.hpp"

void Car::say_hello() {
    std::cout << "Hello, I am a " << this->_brand << " " << _name << "!" << std::endl;
}