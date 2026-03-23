#include <iostream>   // for std::cout
#include <string>     // for std::string
#include <vector>     // for std::vector

class Point {
 private: // By default inside a class everything is private,
          // I still wrote "private:" for demonstration
  
  // Defining a private *member* variable called name;
  std::string name = "undefined";
  double z;

 public: // After this everything will be public (aka
         // accessible from outside this class)
  
  // x and y with default value at 0
  double x = 0.0;
  double y{0.0};

  // Declaring a constructor with some arguments
  // allowing to create a point with different
  // values than the defaults.
  Point(std::string name, double x, double y);
  // Since above we defined a different constructor,
  // the default one was removed.
  // Here we're adding it back.
  Point() = default;

  // Placing this point to a new position
  void place(double x, double y, double z);
  
  // Declaring a print function marked as const,
  // so the function will not modify the point itself
  // This means that this function can be called from
  // variables of type: const Point
  void print() const;

  // Defining a function directly in the class body
  void set_name(std::string name) {
    this->name = name;
  }
};

int main() {
  double x1;
  double y1;
  double x2;
  double y2;

  Point p0; // Created a point with the default constructor
  Point p1("P1", 1.0, 1.0);
  Point p2("P2", -1.0, -1.0);

  // I cannot access to name since is marked as "private"
  // std::cout << p1.name << " " << p1.x << ", " << p1.y << std::endl;
  // A lot of code just to print the two points.
  std::cout << p1.x << ", " << p1.y << std::endl;
  std::cout << p2.x << ", " << p2.y << std::endl;

  // Changing the value of the member variable "name"
  p1.set_name("first point");

  // Calling the print function on every point
  p0.print();
  p1.print();
  p2.print();

  p0.place(0, 0, 0);
  p0.print();

  // Creating a variable of type vector<int>, allowing
  // us to store multiple integer values in the same variable
  std::vector<int> vecInt;
  vecInt.push_back(0);
  vecInt.push_back(1);
  vecInt.push_back(3);
  vecInt.push_back(5);
  // Creating directly a vector with 4 initlial values
  std::vector<int> vecInt2({0, 1, 3, 5});

  // Accessing the vector elements
  for (size_t i = 0; i < vecInt2.size(); i++) {
    std::cout << "index{" << i << "}: " << vecInt2[i] << std::endl;
  }

  // Creating a vector of 4 points.
  // Each one will be constructed using the default constructor
  std::vector<Point> points(4);
  for (size_t i = 0; i < points.size(); i++) {
    points[i].print();
  }

  // Instead of using the default constructor,
  // we can directly use as elements a Point created
  // with different initial values
  std::vector<Point> points2(2, Point("-", 1.0, 1.0));
  // Changing the points inside the vector
  for (size_t i = 0; i < points2.size(); i++) {
    points2[i].set_name("#" + std::to_string(i));
    points2[i].print();
  }

  // Iterating in the points2 vector.
  // Here we are creating a COPY of every element
  // so we are not changing the values in the vector,
  // instead we are changing the value of the copyied
  // version only.
  std::cout << "Second for" << std::endl;
  for (Point element : points2) {
    element.place(0, 0, 0);
    element.print();
  }
  // Here, iterating again will show
  // the unmodified version of the vector elements
  std::cout << "Third for" << std::endl;
  for (Point element : points2) {
    element.print();
  }

  std::cout << "===========" << std::endl;
  // Instead of a copy, we are taking the reference of
  // the element of the vector. A reference is representing
  // the vector element itself. Now if we modify the reference
  // we modify the vector.
  for(Point &element : points2) {
    element.place(10, 10, 10);
    element.print();
  }
  // Demonstrating that the vector changed.
  // Here we are using a constant reference of
  // the point, so we can only call functions that
  // are marked as const. We cannot call the
  // "place" function
  for (const Point &element : points2) {
    element.print();
  }

  // Creating a variable holding a constant integer
  const int a = 0;
  // a = 10; // we cannot modify the value of a

  return 0;
}

// Defining our custom constructor.
Point::Point(std::string name, double x, double y) {
  // instead of
  // this->name = name;
  // this->x = x;
  // this->y = y;
  // We can use our set_name and place functions. Why?
  // to avoid duplicating the same code
  this->set_name(name);
  this->place(x, y, 0.0);
}

// Note the const keyword
void Point::print() const {
  // You can use this-> to explicitly point
  // at the 
  std::cout << name << ": " << x << ", " << this->y << std::endl;
}

// We can use different argument names than the ones that
// we declared in the Point body
void Point::place(double new_x, double new_y, double new_z) {
  x = new_x;
  y = new_y;
  z = new_z;
}