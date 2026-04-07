#include "demo_smart_pointers.hpp"

#include <iostream>
#include <memory>
#include <string>

// This function generates an error:
// I am returning the address of a LOCAL variable.
// Once the function returns, all the variables in the scope
// will be removed, and with that also the age variable.
// When the age variable is removed, the pointer is no longer valid.
std::string *get_age() { // scope
  std::string age = "18";
  return &age;
}

// Class that allows to track when this object is
// created or deleted. Will be used to show what happens to the variables
// the program exits a scope.
class Tracker {
public:
  std::string _name;
  Tracker(std::string name) {
    _name = name;
    std::cout << "[TRACKER constructed] " << _name << std::endl;
  }
  ~Tracker() { std::cout << "[TRACKER destructed] " << _name << std::endl; }
};

void demo_smart_pointers() {
  { // A scope is between two curly braces
    int a;
  }
  {        // This is another scope
    int a; // Note here I can declare another variable with the same name used
           // in the scope above.
  }
  // std::cout << a << std::endl; // I cannot print the a variable since is
  // local to the previous two scopes, not in this one

  double *d_ptr = nullptr;
  {
    double d = 3.14;
    d_ptr = &d; // ERROR: taking address of a local variable!!!
  }
  double d = 10;
  std::cout << *d_ptr << ", " << d
            << std::endl; // Dereferencing can generate an error, in fact using
                          // a DANGLING POINTER is Undefined Behavior

  // ========= HEAP ALLOCATIONS ========= //
  // I want to create a variable in a scope that will exist also when the
  // program will exit that scope. To do this I have to allocate some memory, so
  // that the variable will not be removed automatically.
  {
    d_ptr = new double; // now this is valid
  }
  std::cout << *d_ptr << std::endl; // This is valid
  *d_ptr = 100.0;
  std::cout << *d_ptr << std::endl;
  delete d_ptr; // !! This is a necessary step when allocating memory on the
                // heap. Since we want a "permanent" variable, C++ will not
                // clear the memory for us. If we forgot to clear the memory
                // once we finished using it, then we cause memory leaks. These
                // are when we forgot to clear the variables previously
                // allocated.
                //
                // It can be complicated managing the manual deletion of the
                // variables. Is there an easier way? Yes those are the shared
                // pointers!

  { // Creating a local variable. This is to demonstrate how Tracker works.
    Tracker tracker("Local tracker (I'm on the stack)");
  }
  {
    Tracker *tracker_ptr =
        new Tracker("Leaked tracker (no one will delete me)");
  }

  std::cout << "====== UNIQUE PTR =======" << std::endl;
  // This is a particular type that is keeping track of who is using it. If no
  // one will use this variable, then it will be automatically deleted. In this
  // example The deletion happens at the scope exit.
  std::unique_ptr<Tracker> parent_ptr = nullptr;
  {
    std::unique_ptr<Tracker> u_ptr = std::make_unique<Tracker>(
        "Unique tracker! If no one uses me I will be deleted");
    Tracker *raw_ptr = u_ptr.get();

    // parent_ptr = u_ptr; // Here I am copying the variable. This is NOT
    // ALLOWED since the pointer must be unique (unique_ptr)! How can do? I can
    // move the object

    parent_ptr =
        std::move(u_ptr); // Here I move the local variable to the outer scope!!
    std::cout << " scope exit" << std::endl;
  }

  std::cout << "====== SHARED PTR =======" << std::endl;
  // The shared pointer differs from unique_ptr by the fact that I can have
  // multiple variables pointing to the same object. When none of them will use
  // the pointer, then shared_ptr will delete the memory.
  std::shared_ptr<Tracker> shared_ptr = nullptr;
  {
    std::shared_ptr<Tracker> s_ptr = std::make_shared<Tracker>("SHARED_PTR");
    shared_ptr = s_ptr; // Now I can copy the variable.
    std::cout << " How many copies of shared_ptr exist? " << s_ptr.use_count()
              << std::endl;
    std::cout << " scope exit!" << std::endl;
  }
  // Here I will find only one, since the local variable s_ptr is removed at the
  // scope exit.
  std::cout << " How many copies of shared_ptr exist? "
            << shared_ptr.use_count() << std::endl;
  // Here de object is deleted since the last variable holding the pointer
  // is assigned to nullptr
  shared_ptr = nullptr; // or I could do shared_ptr.reset();

  std::cout << "demo_smart_pointer() scope exit" << std::endl;
}
