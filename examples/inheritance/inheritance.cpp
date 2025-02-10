
/*
 ___       _               _ _
|_ _|_ __ | |__   ___ _ __(_) |_ __ _ _ __   ___ ___
 | || '_ \| '_ \ / _ \ '__| | __/ _` | '_ \ / __/ _ \
 | || | | | | | |  __/ |  | | || (_| | | | | (_|  __/
|___|_| |_|_| |_|\___|_|  |_|\__\__,_|_| |_|\___\___|

C++17 Inheritance and Polymorphism
Author: Davide Stocco, 2025
*/

#include <iostream>
#include <memory> // For smart pointers

// Base class
class Animal {
public:
  virtual void speak() { std::cout << "Animal makes a sound\n"; }
  virtual ~Animal() {} // Virtual destructor for proper cleanup
};

// Derived class - Single Inheritance
class Dog : public Animal {
public:
  void speak() override { std::cout << "Dog barks\n"; }
};

// Demonstrating multiple inheritance
class A {
public:
  void show() { std::cout << "Class A\n"; }
};

class B {
public:
  void show() { std::cout << "Class B\n"; }
};

class C : public A, public B {
public:
  void display() { std::cout << "Class C\n"; }
};

// Abstract class with a pure virtual function
class AbstractAnimal {
public:
  virtual void sound() = 0; // Pure virtual function
  virtual ~AbstractAnimal() {}
};

class Cat : public AbstractAnimal {
public:
  void sound() override { std::cout << "Meow\n"; }
};

// Demonstrating final specifier
class Base {
public:
  virtual void show() { std::cout << "Base class\n"; }
  virtual void display() final { std::cout << "Final function\n"; }
};

class Derived : public Base {
public:
  void show() override { std::cout << "Derived class\n"; }
  // void display() override { std::cout << "Cannot override\n"; } // Error: display() is final
};

// Demonstrating dynamic_cast
class BaseCast {
public:
  virtual ~BaseCast() {} // Required for RTTI
};

class DerivedCast : public BaseCast {
public:
  void show() { std::cout << "Derived class from BaseCast\n"; }
};

int main() {
  // Single Inheritance
  Dog d;
  d.speak();

  // Polymorphism
  Animal* a = new Dog();
  a->speak(); // Calls Dog's speak
  delete a;

  // Multiple Inheritance
  C obj;
  obj.A::show();
  obj.B::show();
  obj.display();

  // Abstract class example
  std::unique_ptr<AbstractAnimal> cat = std::make_unique<Cat>();
  cat->sound();

  // Override and final specifier
  Derived derivedObj;
  derivedObj.show();
  derivedObj.display();

  // dynamic_cast example
  BaseCast* basePtr = new DerivedCast();
  DerivedCast* derivedPtr = dynamic_cast<DerivedCast*>(basePtr);
  if (derivedPtr) derivedPtr->show();
  delete basePtr;

  return 0;
}
