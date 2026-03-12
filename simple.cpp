// A simple C++ program that prints numbers from 1 to 100 and their squares.
// compile with: clang++ simple.cpp -o simple

#include <iostream>

int main() {
  int a;
  for (int i = 1; i <= 100; i++) {
    std::cout << i << " " << i*i << std::endl;
  }

  return 0;
}