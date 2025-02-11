# Basics: Variables, Templates, and Classes

`C++17` introduces various features that improve code readability, efficiency, and maintainability. In this tutorial, we will explore the fundamental aspects.

## Variables

In `C++`, variables can be declared using traditional data types or type inference mechanisms like `auto`. Here are some examples:

```cpp
int a = 10;        // Integer variable
double b = 5.5;    // Double precision floating point
char c = 'A';      // Character variable
std::string s = "Hello"; // String variable
```

`auto` lets the compiler infer the variable type automatically:

```cpp
auto x = 42;       // x is an int
auto y = 3.14;     // y is a double
auto z = "text";  // z is a const char*
```

The curly braces `{}` can be used for initialization:

```cpp
int arr[] = {1, 2, 3}; // Array initialization
std::vector<int> vec = {4, 5, 6}; // Vector initialization
```

### Structured Bindings

`C++17` introduced **structured bindings**, allowing multiple values to be assigned in a single statement:

```cpp
std::tuple<int, double, std::string> data = {1, 2.5, "C++17"};
auto [i, d, str] = data; // i = 1, d = 2.5, str = "C++17"
```

## Templates

Templates allow writing generic and reusable code. They can be used with functions and classes.

### Function Templates

Function templates enable functions to work with different data types:

```cpp
template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << add(5, 10) << std::endl; // Works with integers
    std::cout << add(3.5, 2.1) << std::endl; // Works with doubles
}
```

### Class Templates

Class templates enable generic data types in classes:

```cpp
template <typename T>
class Box {
private:
    T value;
public:
    Box(T val) : value(val) {} // Constructor
    T getValue() { return value; }
};

int main() {
    Box<int> intBox(100);
    Box<std::string> strBox("Hello");
    std::cout << intBox.getValue() << " " << strBox.getValue();
}
```

### Fold expressions

`C++17` introduced **fold expressions**, making it easier to handle parameter packs in variadic templates:

```cpp
template <typename... Args>
auto sum(Args... args) {
    return (args + ...); // Fold expression
}

int main() {
    std::cout << sum(1, 2, 3, 4, 5); // Output: 15
}
```

## Classes

Classes in `C++` encapsulate data and methods. They can have constructors, destructors, methods, and static members. Their aim is to provide a blueprint for objects, which are instances of the class.

A simple class declaration:

```cpp
class Car {
private:
    std::string brand;
    int year;
public:
    Car(std::string b, int y) : brand(b), year(y) {} // constructor
    void display() const { std::cout << brand << " " << year << std::endl; }
};
```

### Constructor and destructor

Constructors initialize an object, while destructors clean up resources.

```cpp
class Example {
public:
    Example() { std::cout << "Constructor called!\n"; }
    ~Example() { std::cout << "Destructor called!\n"; }
};

int main() {
    Example obj; // constructor is called automatically
} // destructor is called when obj goes out of scope
```

### Methods and `this` pointer

Methods define the behavior of a class. The `this` pointer refers to the current object.

```cpp
class Person {
private:
    std::string name;
public:
    Person(std::string n) : name(n) {}
    void introduce() { std::cout << "Hi, I am " << this->name << std::endl; }
};
```

### Static members

Static members belong to the class rather than any specific object. In other words, they are shared among all objects of the class. As a result, they can be accessed using the class name rather than an object. Nonetheless, they cannot access non-static members.

```cpp
class Counter {
private:
    static int count;
public:
    Counter() { count++; }
    static int getCount() { return count; }
};
int Counter::count = 0;
```

## Practice

It is common practice to write the implementation of classes in a separate `.cpp` (or `.cc`, `.cxx`, etc.) file and the declaration in a `.hpp` (or `.h`, `.hh`, etc.) file. This separation helps in maintaining the codebase and improving compilation times.

An example of a class declaration in `Car.hpp`:

```cpp
#ifndef INCLUDE_CAR_HPP // Header guard for Car.hpp
#define INCLUDE_CAR_HPP

#include <iostream> // For std::cout, std::cerr

class Car { // Class declaration
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

  void say_hello(); // Method declaration

  // accessors:
  std::string name() const { return _name; }
  std::string brand() const { return _brand; }

private:
  std::string _name;
  std::string _brand;
  bool _is_started;
};

#endif // INCLUDE_CAR_HPP
```

Here, the implementation of the method `say_hello` in the `Car` class is in `Car.cpp`:

```cpp
#include "Car.hpp"

void Car::say_hello() {
    std::cout << "Hello, I am a " << this->_brand << " " << _name << "!" << std::endl;
}
```