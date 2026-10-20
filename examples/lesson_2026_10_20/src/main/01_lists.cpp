#include <iostream>
#include <list>   // std::list
#include <vector> // std::vector

int main() {
  // Why does a list exist? A vector keeps its elements side by side in
  // memory. When it grows and there is no more room, it allocates a bigger
  // block and MOVES all the elements there:
  // [0, 0, 0, 0, 0, 0, 0]
  //           [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
  // A list instead keeps each element on its own, linked to the next one,
  // so adding elements never moves the existing ones:
  // 0, ...., 0, ...., 0

  std::vector<int> v = {1};
  // std::list is another container of the standard library, used like a
  // vector (push_back, front, back, range-for...). It also has push_front,
  // but no operator[]: to reach element n you have to walk through the list.
  std::list<int> l = {1};
  // & gives the address (where in memory the element lives), we only print it
  std::cout << "vec first element ptr: " << &v.front() << std::endl;
  std::cout << "list first element ptr: " << &l.front() << std::endl;

  // A reference to the first element of the vector. After the push_back
  // below the vector moves its elements, so this reference becomes INVALID
  // (dangling): using it is undefined behaviour. The same happens to
  // addresses and iterators pointing into the vector.
  int &b = v[0];
  for (size_t i = 0; i < 1000; i++) {
    v.push_back(i);
    l.push_back(i);
  }
  // The vector address changed (moved!), the list address is the same
  std::cout << "vec first element ptr: " << &v.front() << std::endl;
  std::cout << "list first element ptr: " << &l.front() << std::endl;

  // Use a list when other things refer to its elements and they must stay
  // put, or when you insert/erase a lot in the middle. Otherwise use a vector.
  // The CNC project keeps its program blocks in a std::list for this reason.

  return 0;
}
