#include<iostream>
#include<functional>
#include<cmath>
#include<vector>
#include<list>

//using namespace std;
//using namespace whatever_1;
//using namespace whatever_2;
using std::cout;
using std::list;

/* LAMBDA FUNCTION
MATLAB syntax : f = @(x) x^2 call f(x_1)
C++17 : []() -> ReturnType {}
  - []: pass the variables from the outside environment to the lambda
  - (): define the variable of the lambda
  - {}: implement the lambda function
*/

// Class with a callback on the destructor
class CustomDeath {
  std::string m_name;
  std::function<void(void)> m_death_callback; 
  // nullptr -> pointer to nothing, useful to uderstand if m_death_callback has been
  // correctly assignes or not. Example:
  // if (m_death_callback != nullptr)

public:
  // Default class constructor
  CustomDeath() 
    : m_name("unassigned"), m_death_callback(nullptr)
  {}
  
  // Class constructor
  CustomDeath(std::string name, std::function<void()> callback) 
    : m_name(name), m_death_callback(callback)
  {}

  // Class desctructor
  ~CustomDeath() { 
    if (m_death_callback) { //Equivalent to: if (m_death_callback != nullptr) {
      m_death_callback();
    }
  }
};

int main() {

  // First lambda function
  std::function<void(void)> f = [](void) -> void {cout << "I'm a lambda!\n";};
  double exponent = 2.1;
  double result; // variable to be modified inside the lambda
  /*std::function<double(double)>*/
  auto power = [exponent, &result](double x) -> double {
    result = std::pow(x, exponent);
    return result;
  };

  /*
  [=] : will pass EVERY varible from outside the lambda by copy
  [&] : will pass EVERY varible from outside the lambda by reference
  */

  // Call the lambda
  f();
  cout << "power(2) = " << power(2) << std::endl;

  // Use custom death class
  CustomDeath silent;
  CustomDeath chatty("chatty", []() { // Equivalent to: [](void) -> void {
    cout << "I'm dead!" << std::endl;
  });

  // STL VECTOR
  std::vector<CustomDeath> obj_vec;
  obj_vec.reserve(10);
  for (size_t i = 0; i < obj_vec.capacity(); ++i)
  {
    // Inefficient way : temporary variable is created and destroyed
    //obj_vec.push_back(CustomDeath("object" + std::to_string(i), [i]() {
    //  cout << "Object " << i  <<" is dead!" << std::endl;
    //}));

    // Efficient way : no temporaries are created!
    obj_vec.emplace_back("object" + std::to_string(i), [i]() {
      cout << "Object " << i  <<" is dead!" << std::endl;
    });
  }

  // STL LIST
  std::list<int> lst1 =  {1, 2, 3};

  // Push and emplace can be done at the beginning and at the end of the list
  lst1.push_front(0);
  lst1.push_back(4);
  lst1.emplace_front(-1);
  lst1.emplace_back(5);
  lst1.emplace_back(3);
  
  // Insertion and removal
  lst1.remove(3); // Removes all elements in the list equal to 3
  auto it1 = lst1.begin();
  std::advance(it1, 3); // get to the fourth element od the list (erase number 2)
  lst1.erase(it1);
  
  // Iterators
  cout << "lst1 =";
  //for (auto it = lst1.begin(); it != lst1.end(); ++it) {
  //  cout << " " << *it;
  //}
  
  for (auto const & i : lst1) {
   cout << " " << i;
  }
  cout << "\n";

  // Merging lists
  std::list<int> lst2 = {0, 0, 1, 2, 3};
  std::list<int> lst3 = {4, 5, 6, 7};
  std::list<int> lst4;
  lst4.merge(lst2);
  lst4.merge(lst3);
  lst4.reverse(); // reverses the order of the list
  lst4.sort(); // ascending order
  lst4.sort([](int i1, int i2) -> bool {
    return i1 > i2;
  });
  lst4.unique(); // removes duplicates concutive elements


  cout << "lst4 =";
  for (auto const & i : lst4) {cout << " " << i;}
  cout << std::endl; // To equivalent to cout << "\n";
  
  // Splicing of lists: moving elemnts from one list to the other
  auto it2 = lst2.begin();
  std::advance(it2, lst2.size()-2);
  lst3.splice(it2, lst2);

  cout << "lst2 =";
  for (auto const & i : lst2) {cout << " " << i;}
  cout << std::endl; // To equivalent to cout << "\n";
  cout << "lst3 =";
  for (auto const & i : lst3) {cout << " " << i;}
  cout << std::endl; // To equivalent to cout << "\n";






  

  return 0;
}