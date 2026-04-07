#include <iostream>

// One possible implementation of square by copying both
// the argument and the returned value
double square_copy(double number) {
  return number * number;
}
// Another possible implementation using instead the references.
// No copy involved. It becomes useful when the objects passed as
// argument are big.
void square(double &number) {
  number = number * number;
}
// Another use of references: possibility to modify multiple
// values from a single function.
void scale(double s, double &x, double &y) {
  x = s * x;
  y = s * y;
}

// Here we will do some bad things to break the code
void break_code();

int main() {
  double age = 21;
  double &reference_age = age;
  std::cout << age << " " << reference_age << std::endl;
  reference_age = 22;
  std::cout << age << " " << reference_age << std::endl;
  square(age);
  std::cout << "age after square: " << age << std::endl;

  double x = 10.0, y = 10.0;
  scale(2, x, y);
  std::cout << "x: " << x << " y: " << y << std::endl;
  
  // Here I would like to change the object at which the
  // reference is acting on, however this is not possible,
  // so actually I will change the value of the object
  // on which I created the reference on.
  double second_age = 35;
  reference_age = second_age;
  reference_age = 22;
  std::cout << "age: " << reference_age << std::endl;
  std::cout << "age: " << reference_age << " second_age: " << second_age << std::endl;

  // Pointers
  // pointer can be not initialized to an object
  // You should always initialize it to nullptr.
  double *pointer_age;
  pointer_age = &age; // take the address of age and assigning it to pointer_age
  std::cout << pointer_age << " == " << &age << std::endl;
  std::cout << pointer_age << " age: " << *pointer_age << std::endl;
  *pointer_age = 18; // Dereferencing the pointer to modify age.
  std::cout << *pointer_age << " age: " << age << std::endl;

  // Writing this is an error, here I am trying to set the address
  // to 18. AKA the 18th byte in the memory. This should not be done
  // unless you know what you're doing.
  // reference_age = 18;

  // Pointer can change the variable at which they point to.
  pointer_age = &second_age;
  std::cout << *pointer_age << " age: " << age << std::endl;

  // Just to keep the main clear
  break_code();

  return 0;
}

// Returning the pointer of a local variable.
// This is an ERROR, since the pointer is valid
// only inside the function, once the function returns
// the LOCAL variable age is destroyed, thus invalidating
// the pointer
std::string *get_age() {
  std::string age = "-";
  return &age;
}

// Same error as above, we are taking the reference of a local variable.
std::string &get_name() {
  std::string name = "Pippo";
  return name;
}

void break_code() {
  // Here you can try the difference between initializing 
  // x_ptr to nullptr or not. By setting it to nullptr
  // we will not enter in the if, instead if we do not initialize it,
  // it is POSSIBLE (different pc or even the same pc in different times can
  // behave differently) that we enter the if and we make the program crash
  // when dereferencing the pointer
  double *x_ptr = nullptr;
  if (x_ptr) {
    std::cout << x_ptr << std::endl;
    std::cout << *x_ptr << std::endl; // Here is a possible crash
  }
  std::cout << "---" << std::endl; // just to check wether we passed the if
  
  std::string *age_ptr = get_age(); // This address is invalid here
  std::cout << age_ptr << std::endl; // This could still work
  if(age_ptr) { // this will work
    std::cout << "age from get_age: " << *age_ptr << std::endl; // this can crash
    *age_ptr = "aaaaaaaaaaaaaaaa"; // also here
  }

  std::cout << get_name() << std::endl;
  std::string &name = get_name();
  name = "new name is pluto";
  std::cout << name << std::endl;
}
