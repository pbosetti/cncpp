#include <iostream> // use std::cout
#include <vector> // use std::vector

// Most generic class
class Animal {
  std::string m_sound{"undefined"};

public:
  // Class deconstructor
  ~Animal() {
    std::cout << "Ahhhh nooo" << std::endl;
  }

  // Class constructor
  Animal() {}

  // Class constructor 1
  Animal(std::string s)
    : m_sound(s)
  {
    // this->m_sound = s;
  }

  // Function that return the sound string
  const std::string & // privide the reference (memory position) of this->m_sound
  sound() const { 
    return this->m_sound;
  }

  // Function to make a sound
  void make_sound() {
    std::cout << "This animal sounds like " << this->m_sound << std::endl;
  }

  // Function to make a sound (overload)
  // Static functions do not depend on the class members
  static void make_sound(std::string s) {
    std::cout << "This animal sounds like " << s << std::endl;
  }
};

// Class Lion
class Lion : public Animal {
public:
  // Class constructor
  Lion() : Animal("Roar roar") {}

  // Function to make a sound (override)
  // Static functions do not depend on the class members
  static void make_sound(std::string s = "Roar") { // Default variable
    std::cout << "This animal sounds like " << s << std::endl;
  }
};

// class Goat
class Goat : public Animal {
public:
  Goat() : Animal("Beee") {}
};

// class Snake
class Snake : public Animal {
public:
  Snake() : Animal("Bsss") {}
};

// class Chimera
class Chimera final : // derived or child class
public Lion, public Goat, public Snake // base or parents classes
{
public:
  Chimera() {}

    // Function to make a sound
    void make_sound() {
      std::cout << "The chimera sounds like " << 
       this->Lion::sound() << ", " /*
       Referring to this class, go inside the Lion inherited class and retrive 
       the sound string
       */
       << this->Goat::sound() << ", and "
       << this->Snake::sound() << "!"
       << std::endl;
    }
};

//class More_than_chimera : public Chimera {
//public:
//  More_than_chimera(){}
//};

// VIRTUAL/ASBRACT CLASSES ---------------

// Virtual class
class AnimalVirtual {
public:
  AnimalVirtual(){} // non-virtual class constructor

  virtual void make_sound() const = 0; // pure-virtual method
};

// Inherit for a virtual class
class LionNonVirtual : public AnimalVirtual {
public:
  LionNonVirtual() = default; // just another way of providing the class constructor

  // Override the make_sound
  void make_sound() const override {
    std::cout << "Roar" << std::endl;
  }
};

class GoatNonVirtual : public AnimalVirtual {
  public:
  GoatNonVirtual() = default; 
  
    // Override the make_sound
    void make_sound() const override {
      std::cout << "Beeeeh" << std::endl;
    }
  };
  
// Define a class that use AnimalNonVirtual and call make_soud
void animal_sound(AnimalVirtual const * a) {
  a->make_sound(); //(*a).make_sound();
}

int main()
{
  // Create an "empty" animal object
  Animal animal_1; // Equivalent to Animal animal_1();
  animal_1.make_sound();

  // Create an "empty" animal object
  Animal animal_2("meow");
  animal_2.make_sound();

  // Access the static function
  Animal::make_sound("wof");
  Lion::make_sound();

  // Use Lion class
  Lion scar;
  scar.make_sound(); // make_sound is already contained in Animal

  // use chimera
  Chimera chimera;
  chimera.make_sound();

  // use More_than_chimera
  //More_than_chimera trial; -> ERROR

  // use AnimalVirtual
  // AnimalVirtual virtual_animal; -> ERROR

  LionNonVirtual lion;
  lion.make_sound();

  GoatNonVirtual goat;
  goat.make_sound();

  animal_sound(&lion);
  animal_sound(&goat);

  // VECTORS - CONTINUED

  std::vector<double> v{1.0, 2.0, 3.0, 4.0, 5.0};

  // std::vector<Class<double, 3, Derived<4, int>>>
  
  // Basic way to access the elements of a vecto in a forr loop
  for (int i{0}; i < v.size(); ++i) {
    std::cout << v.at(i) << std::endl;
  }

  // Use more advanced techniques...
  for (double & v_i : v) {
    ++v_i;
    std::cout << v_i << std::endl;
  }

  for (double const & v_i : v) { // this does not allow to modify the vector v
    std::cout << v_i << std::endl;
  }

  for (auto const & v_i : v) { // this does not allow to modify the vector v
    std::cout << v_i << std::endl;
  }

  // Access through iterators
  std::vector<double>::iterator it;
  it = v.begin();
  std::cout << "The variable it has a value of " << *it << std::endl;

  // Forward iterator
  for (auto itr = v.begin(); itr != v.end(); ++itr) {
    std::cout << "itr1 = "<<  *itr << std::endl;
  }

  // Erase and insert the penultimate element
  v.erase(v.end()-2, v.end()-1); // deletes entry v[5]
  v.insert(v.end()-1, 5.0); // insert again v[5] = 5

  // Reverse iterator
  for (auto itr = v.rbegin(); itr != v.rend(); ++itr) {
    std::cout << "itr2 = "<<  *itr << std::endl;
  }

  // Detes everything
  v.clear();

  return 0;
}

/* Example
Try to complete the class triangle with the proper function implementation, and 
implement a class rectangle. Then write a funcion that computes the difference
between the areas of 2 polygons.

IMPORTANT: USE A std::vector to store the polygon points!

class Polygon{
public:
  Polygon() {}
  virtual double area() = 0; 
  virtual double perimeter() = 0; 
};

class Triangle : public Polygon {
  using std::vector<double> = vec;
  vec m_point_1;
  vec m_point_2;
  vec m_point_3;

  public:
  Triangle() : m_point_1(2), m_point_2(2), m_point_3(2) 
  {}

  Trianglevec p1, vec p2, vec p3) :
   : m_point_1(p1), m_point_2(p2), m_point_3(p3) 
  {
   }

  double area() override {
    return x_1*x_2; // COMPLETE
  }

  double perimeter() override {
    return x_1*x_2; // COMPLETE
  }
};*/