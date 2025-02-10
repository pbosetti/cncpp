# Inheritance and polymorphism

Inheritance and polymorphism are fundamental concepts in object-oriented programming that allow code reuse, extension, and dynamic behavior.

- **Inheritance** enables a new class (derived class) to acquire the properties and behavior of an existing class (base class).
- **Polymorphism** allows methods to have different behaviors based on the object that calls them, often achieved using **virtual functions**.

This tutorial explores different types of inheritance, method overriding, `virtual` functions, and `C++17` enhancements like `override` and `final` specifiers.

## Inheritance

Inheritance establishes a relationship between classes where a **derived class** inherits the attributes and methods of a **base class**.

```cpp
#include <iostream>
class Animal {
public:
    void speak() { std::cout << "Animal makes a sound\n"; }
};

class Dog : public Animal {
public:
    void bark() { std::cout << "Dog barks\n"; }
};

int main() {
    Dog d;
    d.speak(); // Inherited from Animal
    d.bark();
    return 0;
}
```
### Types of inheritance

Inheritance can be of different types:

| Type | Syntax | Access in Derived Class |
|-------|--------|----------------------|
| Public | `class Derived : public Base` | Public & Protected members remain the same |
| Protected | `class Derived : protected Base` | Public members become Protected |
| Private | `class Derived : private Base` | Public & Protected members become Private |

```cpp
class Base {
protected:
    int x = 10;
};

class Derived : private Base {
public:
    void show() { std::cout << x << "\n"; } // Allowed due to protected access
};
```

## Function overriding & virtual functions

By default, when a derived class defines a method with the same name as the base class, it hides the base class method.

```cpp
class Base {
public:
    void show() { std::cout << "Base class\n"; }
};
class Derived : public Base {
public:
    void show() { std::cout << "Derived class\n"; }
};

int main() {
    Derived d;
    d.show(); // Derived class

    Base* b = &d;
    b->show(); // Base class (not overridden, method hiding)
}
```

To enable polymorphic behavior, we use **virtual functions**. A **virtual function** ensures that the correct function is called for an object, even if accessed via a base class pointer.

```cpp
class Base {
public:
    virtual void show() { std::cout << "Base class\n"; }
};

class Derived : public Base {
public:
    void show() override { std::cout << "Derived class\n"; }
};

int main() {
    Base* b = new Derived();
    b->show(); // Output: Derived class
    delete b;
}
```

### `override` and `final` specifiers

- `override` ensures the method actually overrides a base class method.
- `final` prevents further overriding in derived classes.

```cpp
class Base {
public:
    virtual void show() override { std::cout << "Base class"; } // Error: No function to override
    virtual void display() final { std::cout << "Final function"; }
};

class Derived : public Base {
public:
    void display() override { std::cout << "Cannot override"; } // Error: display() is final
};
```

## Abstract classes & pure virtual functions
An **abstract class** contains at least one **pure virtual function**, which must be overridden in derived classes.

```cpp
class Animal {
public:
    virtual void sound() = 0; // Pure virtual function
};

class Dog : public Animal {
public:
    void sound() override { std::cout << "Bark\n"; }
};
```

## Multiple Inheritance
`C++` allows multiple inheritance but requires care to avoid ambiguity.
```cpp
class A {
public:
    void show() { std::cout << "Class A\n"; }
};
class B {
public:
    void show() { std::cout << "Class B\n"; }
};
class C : public A, public B {
};

int main() {
    C obj;
    obj.show(); // Error: Ambiguous call to show()
}
```
To resolve ambiguity:
```cpp
obj.A::show();
```

## `dynamic_cast` and run-time type identification

Using `dynamic_cast` safely converts a base class pointer to a derived class pointer. Notice that an overhead is involved.

```cpp
class Base {
public:
    virtual ~Base() {} // Required for RTTI
};
class Derived : public Base {
public:
    void show() { std::cout << "Derived class\n"; }
};

int main() {
    Base* b = new Derived();
    Derived* d = dynamic_cast<Derived*>(b);
    if (d) d->show();
    delete b;
}
```

## Summary

| Concept | Description |
|---------|-------------|
| Inheritance | Code reuse by deriving classes |
| Virtual Function | Enables polymorphism |
| Override | Ensures proper method overriding |
| Final | Prevents further overriding |
| Abstract Class | Contains at least one pure virtual function |
| Multiple Inheritance | Allows a class to inherit from multiple classes |
