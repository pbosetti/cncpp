/*
 ____ _____ _      __  __
/ ___|_   _| |    |  \/  | __ _ _ __
\___ \ | | | |    | |\/| |/ _` | '_ \
 ___) || | | |___  | |  | | (_| | |_) |
|____/ |_| |_____| |_|  |_|\__,_| .__/
                |_|
Standard Template Library (STL) Map
Author: Davide Stocco, 2025
*/

#include <iostream>
#include <map>
#include <unordered_map>

struct Descending {
  bool operator()(int a, int b) const {
    return a > b; // Sort in descending order
  }
};

int main(int argc, char * argv[])
{
  // Declaring and Initializing Maps
  std::map<int, std::string> students = {
    {101, "Alice"},
    {102, "Bob"},
    {103, "Charlie"}
  };

  std::map<std::string, int> wordCount; // Empty map

  // Accessing Elements
  std::cout << "Student 101: " << students[101] << std::endl;
  std::cout << "Student 102: " << students.at(102) << std::endl;

  // Checking if a Key Exists
  if (students.find(105) != students.end()) {
    std::cout << "Student 105 exists!" << std::endl;
  } else {
    std::cout << "Student 105 does not exist." << std::endl;
  }

  // Iterating Through a Map
  std::cout << std::endl << "Iterating with iterator:" << std::endl;
  for (auto it = students.begin(); it != students.end(); ++it) {
    std::cout << "ID: " << it->first << ", Name: " << it->second << std::endl;
  }

  std::cout << std::endl << "Iterating with range-based loop:" << std::endl;
  for (const auto& [id, name] : students) {
    std::cout << "ID: " << id << ", Name: " << name << std::endl;
  }

  // Lower and Upper Bounds
  auto lb = students.lower_bound(102);
  auto ub = students.upper_bound(102);

  std::cout << std::endl << "Lower Bound of 102: " << lb->first << " - " << lb->second << std::endl;
  if (ub != students.end()) {
    std::cout << "Upper Bound of 102: " << ub->first << " - " << ub->second << std::endl;
  } else {
    std::cout << "Upper Bound of 102 does not exist." << std::endl;
  }

  // Adding and Modifying Elements
  students[104] = "David"; // Inserts or modifies
  students.insert({105, "Emma"}); // Insert method

  auto result = students.insert({102, "Eve"});
  if (!result.second) {
    std::cout << "Insertion failed: Key 102 already exists" << std::endl;
  }

  // Removing Elements
  students.erase(101); // Remove by key
  students.erase(students.begin()); // Remove by iterator

  // Checking Size and Empty State
  std::cout << std::endl << "Map size: " << students.size() << std::endl;
  if (students.empty()) {
    std::cout << "Map is empty!" << std::endl;
  }

  // Swapping Two Maps
  std::map<int, std::string> map1 = {{1, "A"}, {2, "B"}};
  std::map<int, std::string> map2 = {{3, "X"}, {4, "Y"}};

  map1.swap(map2);
  std::cout << std::endl << "After swapping, map1 contains:" << std::endl;
  for (const auto& [id, name] : map1) {
    std::cout << "ID: " << id << ", Name: " << name << std::endl;
  }

  // Using a Custom Comparator
  std::map<int, std::string, Descending> rev_map = {
    {1, "One"},
    {2, "Two"},
    {3, "Three"}
  };

  std::cout << std::endl << "Descending order map:" << std::endl;
  for (const auto& [id, name] : rev_map) {
    std::cout << "ID: " << id << ", Name: " << name << std::endl;
  }

  // Using Unordered Map (Faster Alternative)
  std::unordered_map<int, std::string> u_map;
  u_map[1] = "Alice";
  u_map[2] = "Bob";

  std::cout << std::endl << "Unordered Map:" << std::endl;
  for (const auto& [id, name] : u_map) {
    std::cout << "ID: " << id << ", Name: " << name << std::endl;
  }

  return 0;
}
