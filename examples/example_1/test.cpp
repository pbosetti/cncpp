#include <iostream>
#include <string>

// To compile the code
// cmake --build build
// To run this example
// ./build/examples/example_1

// For C++ reference use
// cppreference.com


bool check_age(int age) {
  if (age >= 18) {
    return true;
  } else {
    return false;
  }
}

std::string age_type(int age) {
  if (age >= 18) {
    return "adult";
  } else {
    return "not adult";
  }
}

// Declaring the sum function
float sum(float a, float b);
// Declaring another sum function but
// using double instead of float
double sum(double a, double b);

// Declaring a template function with only
// one template parameter (typename T)
template <typename T>
T templated_sum(T a, T b);
// Declaring a *specialization* of the
// templated_sum declared above
template <>
int templated_sum<int>(int a, int b);

int main() {

  int a = 10;
  float b = 20.1f;
  double c = 20.0;
  char character = 'c';

  int my_age = 20;

  if(my_age > 18) {
    character = 'm';
  } else {
    character = '-';
  }

  std::cout << "charater: " << character << std::endl;

  // String containing text.
  // Special characters (the correct name is "escape character"):
  // - "\n" inserts a New line
  // - "\t" inserts a Tab
  std::string line_of_text =
  "Content of the line.\nContent of the second line.\n\t- Item 1";

  std::cout << line_of_text << std::endl;

  // This is a comment

  /*
    Multi
    Line
    Comment
  */

  // Checking directly the return value of a function
  if (check_age(my_age)) {
    std::cout << "adult" << std::endl;
  }

  // Printing directly the return value of a function
  std::cout << age_type(my_age) << std::endl;

  // Demonstraing that calling sum with two floats
  // Will call the float implementation
  float x1 = 1.0;
  float x2 = 2.0;
  float x3 = sum(x1, x2);

  // Same but with doubles
  double y1 = 1.0;
  double y2 = 2.0;
  double y3 = sum(y1, y2);

  // Both are calling the generic implementation
  // of the templated_sum function
  x3 = templated_sum(x1, x2);
  y3 = templated_sum(y1, y2);
  
  // Calling the specialized version of
  // templated_sum that accepts integers
  int my_int = templated_sum<int>(0, 1);
  // Forcing to call the float implementation
  // even if the arguments (0, 1) are integers
  my_int = templated_sum<float>(0, 1);

  return 0;
}

// Definition (implementation) of the
// float version of the sum function
float sum(float a, float b) {
  std::cout << "float sum" << std::endl;
  return a + b;
}
// Same but with doubles
double sum(double a, double b) {
  std::cout << "double sum" << std::endl;
  return a + b;
}

// Defining the generic implementation
// of templated_sum
template <typename T>
T templated_sum(T a, T b) {
  return a + b;
}
// Defining the SPECIALIZATION of the
// generic templated_sum
template <>
int templated_sum<int>(int a, int b) {
  std::cout << "called templated_sum<int>" << std::endl;
  return a + b;
}