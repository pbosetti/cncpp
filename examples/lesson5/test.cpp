#include<iostream>
#include<map>
#include<unordered_map> // to include std::unordered_map
#include<utility> // to include std::pair

// FMT (format) library (add LIBS fmt::fmt to link the dynamic library file)
#include<fmt/core.h>

// Rang (header-only) 
#include<rang.hpp>

// Include some objects/function from the std namespace
using std::cout;
using std::endl;
using std::map;
using std::unordered_map;
using std::string;
using std::pair;

// Descending rule for map
struct Desending {
  bool operator() (int a, int b) const {
    return a > b;
  }
};

// Struct point
struct Point {
  string name;
  double x, y;

  Point operator+ (const Point & p2) const {
    Point tmp = {
      this->name + p2.name,
      this->x + p2.x,
      this->y + p2.y
    };
    return tmp;
  }
};

// Define and fmt print rule the Point objects
template <>
class fmt::formatter<Point> {
public:
  constexpr auto parse(format_parse_context & ctx) {return ctx.begin();}

  template <typename Context>
  constexpr auto format(Point const & p, Context & ctx) const {
    return format_to(ctx.out(), "{}({:.2f}, {:.2f})", p.name, p.x, p.y);
  }
};

// Add function definition
template <typename T>
T add(T a, T b) {
  return a + b;
}

// Integer power function
template<int exp, typename T>
T ipow(T base) {
  static_assert(exp >= 1, "exponent must be >= 1");
  T res = base;
  //if (exp > 1) { // Evaluated at run time ./examples/lesson5
  if constexpr (exp > 1) { // Evaluated at compile time!
    if constexpr (exp == 2) {
      res *= base;
    } else if constexpr (exp > 2) {
      for (int i{0}; i < exp-1; ++i){
        res *= base;
      }
    }
  }
  return res;
}

// Main function
int main() {

  // Map object to map from a stundent ID to their name
  map<int, string> students_id2name = {
    {100, "Davide"},
    {101, "Anna"},
    {103, "Otto"},
    {104, "Jacques"},
    {105, "Bob"}
  };
  map<string, int> students_name2id = {
    {"Davide", 100},
    {"Anna", 101},
    {"Otto", 103},
    {"Jacques", 104},
    {"Bob", 105}
  };

  // Query the map ("unsafely")
  cout << "Student with ID 101: " << students_id2name[101] << endl;
  cout << "Student with name ""Anna"": " << students_name2id["Anna"] << endl;

  // Query the map ("safely" with .at(i))
  // If 'i' is not not cointained in the map than an exception (error) is throw
  cout << "Student with ID 101: " << students_id2name.at(101) << endl;
  cout << "Student with name ""Anna"": " << students_name2id.at("Anna") << endl;

  // Insert a new key-value pair
  students_id2name.insert({106, "Charlie"});

  // Erase a key-value pair
  students_id2name.erase(106);

  // Check if a key exists
  if (students_id2name.find(106) != students_id2name.end()) {
    cout << "Key exists!" << endl;
  } else {
    cout << "Key does not exist!" << endl;
  }

  // Iterating through the map
  cout << "Iterator technique" << endl;
  for (auto it = students_id2name.begin(); it != students_id2name.end(); ++it) {
    cout << "ID: " << (*it).first << ", name: " << it->second << endl;
  }

  cout << """Auto"" technique" << endl;
  for (const auto & [k, v] : students_id2name) {
    cout << "ID: " << k << ", name: " << v << endl;
  }

  // Pairs
  // pair<int, string> p1 = {0, "Empty"};
  // p1.first; p1.second; Access the 1st and 2nd elements
  // Tuple n-elements
  // tuple<int, string, ..., double> t1 = {1, "2", ..., 3.0}
  // std::get<0>(t1), std::get<1>(t1), ..., std::get<n-1>(t1) Access the elements

  // Swapping maps values
  map<int, string> map1 = {{1, "A"}, {2, "B"}}; 
  map<int, string> map2 = {{3, "C"}, {4, "D"}}; 
  map1.swap(map2);

  // Sorting a map
  map<int, string, Desending> map3 = {
    {1, "A"},
    {2, "B"},
    {3, "C"}
  };
  cout << "Sorting" << endl;
  for (const auto & [k, v] : map3) {
    cout << "key: " << k << ", value: " << v << endl;
  }

  // Unordered maps
  unordered_map<int, string> map_u;
  map_u[1] = "A";
  map_u[2] = "B";
  map_u[0] = "B";
  for (const auto & [k, v] : map_u) {
    cout << "key: " << k << ", value: " << v << endl;
  }

  // Usage of FMT "fmt::format"
  string name{"Davide"};
  int age{30};
  string s1 = fmt::format("My name is {}, and I am {}, years old.", name, age);
  cout << s1 << endl;

  // Usage of FMT "fmt::print"
  fmt::print("My name is {}, and I am {}, years old.\n", name, age);

  // Aligment
  double num = 3.0;
  fmt::print("My number is {:>5} (right-aligned '>' and '5' chars wide)\n", num);
  fmt::print("My number is {:<5} (left-aligned '>' and '5' chars wide)\n", num);

  // Precision
  fmt::print("My number is {:.2f} ('2' numbers after the dot)\n", num);

  // Point
  Point p1 = {"P1", 2.0, 3.0};
  Point p2 = {"P2", 2.0, 3.0};
  Point p3 = p1 + p2;
  fmt::print("My point is: {}\n", p3);

  // Rang libay usage
  cout << rang::bg::yellow << rang::style::bold << "Hello!" 
    << rang::style::reset
    << endl;

  // rang::bg::'color': modify the background color
  // rang::fg::'color': modify the foreground color
  // rang::style::'italic/bold': modify the font style
  // rang::style::reset: go back to the default

  // Combine rang with fmt
  cout << rang::fg::cyan << rang::style::bold 
    << fmt::format("This is a cyan fg!") 
    << rang::style::reset
    << endl;

  // Integer power
  cout << "ipow<2>(2) = " << ipow<2>(2) << endl;

  return 0;
}