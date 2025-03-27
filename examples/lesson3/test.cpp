#include <iostream> // use std::cout

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

  return 0;
}


/* Example
class Polygon{
public:
  Polygon() {}
  virtual double area() = 0; 
  virtual double perimeter() = 0; 
};

class Triangle : public Polygon {
  double x_1;
  double y_1;
  double x_2;
  double y_2;
  double x_3;
  double y_3;

  public:
  Triangle(){}

  double area() override {
    return x_1*x_2;
  }

  double perimeter() override {
    return x_1*x_2;
  }
};*/