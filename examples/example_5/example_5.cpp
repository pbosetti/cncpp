#include "demo_smart_pointers.hpp"
#include <iostream>
#include <memory>
#include <vector>

class Shape {
public:
  Shape(std::string name) { _name = name; }
  virtual ~Shape() {}
  void print() { std::cout << _name << std::endl; }
  virtual void num_of_sides() { std::cout << "undefined" << std::endl; }

protected:
  std::string _name;
};
class Circle : public Shape {
public:
  Circle() : Shape("circle") {}
  void num_of_sides() override {
    std::cout << _name << " has infinite sides" << std::endl;
  }
};
class Triangle : public Shape {
public:
  Triangle() : Shape("triangle") {}
  void num_of_sides() override {
    std::cout << _name << " has 3 sides" << std::endl;
  }
};

int main() {
  std::cout << "\n== Demo on smart pointers" << std::endl;
  demo_smart_pointers();

  std::cout << "\n== Demo on polymorphism" << std::endl;

  std::vector<std::unique_ptr<Shape>> shapes;
  std::unique_ptr<Circle> c1 = std::make_unique<Circle>();
  shapes.push_back(std::move(c1));
  std::unique_ptr<Triangle> t1 = std::make_unique<Triangle>();
  shapes.push_back(std::move(t1));

  for (size_t i = 0; i < shapes.size(); i++) {
    shapes[i]->print();
    shapes[i]->num_of_sides();
  }

  return 0;
}
