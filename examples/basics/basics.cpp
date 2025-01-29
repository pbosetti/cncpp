#include <iostream>
#include "myobject.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  cout << "Hello, World!" << endl;

  MyObject obj = MyObject("Paolo");
  obj.say_hello();

  cout << "in obj, _name = \"" << obj.name() << "\"" << endl;

  return 0;
}