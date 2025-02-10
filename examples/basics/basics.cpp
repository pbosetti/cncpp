/*
 ____            _
| __ )  __ _ ___(_) ___ ___
|  _ \ / _` / __| |/ __/ __|
| |_) | (_| \__ \ | (__\__ \
|____/ \__,_|___/_|\___|___/

C++17 Basics Examples
Author: Davide Stocco, 2025
*/

#include <iostream>
#include <vector>
#include <tuple>
#include "Car.hpp"

// Function Template
template <typename T>
T add(T a, T b) {
    return a + b;
}

// Variadic Template with Fold Expression
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

// Class Template
template <typename T>
class Box {
private:
    T value;
public:
    Box(T val) : value(val) {}
    T getValue() { return value; }
};

// Example Class
template <typename T>
class Example {
public:
    Example() { std::cout << "Constructor called!\n"; }
    ~Example() { std::cout << "Destructor called!\n"; }
};

int main() {
    // Variable Declarations
    int a = 10;
    double b = 5.5;
    char c = 'A';
    std::string s = "Hello";
    auto x = 42;
    auto y = 3.14;
    auto z = "text";
    std::vector<int> vec = {4, 5, 6};
    std::tuple<int, double, std::string> data = {1, 2.5, "C++17"};
    auto [i, d, str] = data;

    // Template Examples
    std::cout << add(5, 10) << std::endl;
    std::cout << add(3.5, 2.1) << std::endl;
    std::cout << sum(1, 2, 3, 4, 5) << std::endl;

    // Class Template Usage
    Box<int> intBox(100);
    Box<std::string> strBox("Hello");
    std::cout << intBox.getValue() << " " << strBox.getValue() << std::endl;

    // Object Example
    Example<int> obj;

    // Car Class Usage
    Car myCar("Model S", "Tesla");
    myCar.say_hello();
    myCar.start();
    myCar.stop();

    return 0;
}
