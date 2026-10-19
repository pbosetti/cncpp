#include "thermostat.hpp"
#include <iostream>

// Passing by value: the function receives a COPY of the variable,
// so the variable of the caller is not modified
void add_one_copy(int x) {
  x = x + 1;
}
// Passing by reference (&): the function works on the ORIGINAL variable
void add_one(int &x) {
  x = x + 1;
}

// Same thing with objects. Here the copy is even worse, since
// we are copying the whole thermostat (name, setpoint, ...)
// and then all the changes are lost at the end of the function
void cold_snap_copy(Thermostat t) {
  t.set_setpoint(22.0);
  t.update(10.0);
}
void cold_snap(Thermostat &t) {
  t.set_setpoint(22.0);
  t.update(10.0);
}

// Passing by const reference: no copy, and the object is read-only.
// We can call only the methods marked as const, this is why last lesson
// we made print() and the getters const
void report(const Thermostat &t) {
  // t.set_setpoint(19.0); // Cannot call since set_setpoint is not const
  t.print();
}

// Rule of thumb:
// - small types (int, double, bool) by value
// - objects by const &
// - & only when the function has to modify the argument

int main() {
  // A reference is another name for an existing variable.
  // Modifying r means modifying a
  int a = 5;
  int &r = a;
  r = 10;
  std::cout << "a = " << a << " r = " << r << std::endl;
  // A reference must be initialized, and it cannot be "moved"
  // to another variable later
  // int &bad;

  // n is still 0 after add_one_copy, and 1 after add_one
  int n = 0;
  add_one_copy(n);
  std::cout << "after add_one_copy: n = " << n << std::endl;
  add_one(n);
  std::cout << "after add_one: n = " << n << std::endl;

  // The first report prints OFF with setpoint 21 (we changed a copy),
  // the second one prints ON with setpoint 22 (we changed lab)
  Thermostat lab("lab", 21.0, 0.5);
  cold_snap_copy(lab);
  report(lab);
  cold_snap(lab);
  report(lab);

  return 0;
}
