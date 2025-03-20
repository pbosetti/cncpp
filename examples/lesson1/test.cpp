#include <string>
#include <iostream>

// namespace std
// using namespace std;
using std::string;

// Function add declaration
template <typename Mytype>
Mytype add(Mytype a, Mytype b);

// Sum n times the same variable
template <typename T>
T sum(T a, int N) {
  T tmp = 0.0;
  for (int i{0}; i < N; ++i) {tmp += a;}
  return tmp;
}

template <typename T, unsigned int N>
T sum(T a) {
  T tmp = 0.0;
  for (int i{0}; i < N; ++i) {tmp += a;}
  return tmp;
}

int main(void) {
  // Here is a single line comment
  /*
  Multi
  line
  comment
  */
  int    my_var = 2;
  float  my_float = 3.0f;
  double my_double = 4.0;
  char   my_char = 'a';

  string my_string("Hello!");
  std::cout << my_string << " Davide" << std::endl;

  double n1, n2;
  n1 = n2 = 2.0;
  double n3 = add<double>(n1, n2);

  int n1i, n2i;
  n1i = n2i = 2;
  int n3i = add<int>(n1i, n2i);
  std::cout << "The sum between " << n1 << " and " << n2 << " is " << n3 << std::endl;

  
  n3 = sum<double>(n1, 10);
  n3 = sum<double, 10>(n1);

  return 0;
} 

// Add function definition
template <typename T>
T add(T a, T b) {
  return a+b;
}
