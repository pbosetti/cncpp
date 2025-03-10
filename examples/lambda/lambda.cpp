/*
 _                    _         _
| |    __ _ _ __ ___ | |__   __| | __ _
| |   / _` | '_ ` _ \| '_ \ / _` |/ _` |
| |__| (_| | | | | | | |_) | (_| | (_| |
|_____\__,_|_| |_| |_|_.__/ \__,_|\__,_|

Lambda functions
Author: Paolo Bodetti, 2025
*/

#include <iostream>
#include <thread>
#include <functional>

using namespace std;

class CustomDeath {
public:
  // Designated constructor:
  CustomDeath(string name, function<void()> death_callback) : name(name), _death_callback(death_callback) {
    cout << name << ": This object will die with a callback" << endl;
  }

  // Using designated constructor
  // Note that std::function is actually a pointer to a function
  CustomDeath() : CustomDeath("unnamed", nullptr) {}
  CustomDeath(string name) : CustomDeath(name, nullptr) {}

  // Destructor:
  ~CustomDeath() {
    // Only use the callback if defined (not nullptr!):
    if (_death_callback) _death_callback();
  }

  string name;

  private:
  function<void()> _death_callback = nullptr;
};


int main() {
  CustomDeath silent = CustomDeath("silent");

  // Lambdas have a capture list, which is a way to pass variables to the lambda function. The capture
  // list is inside the square brackets [], which can be empty, or contain variables to be passed.
  // The capture list can be passed by value or by reference, e.g.:
  // - [&] means pass all variables by reference
  // - [=] means pass all variables by value
  // - [var1, var2] means pass var1 and var2 by value
  // - [&var1, &var2] means pass var1 and var2 by reference
  CustomDeath chatty = CustomDeath("chatty", [&]() {
    cout << chatty.name << ": Oh, no! I'm being released!" << endl;
  });

  return 0;
}