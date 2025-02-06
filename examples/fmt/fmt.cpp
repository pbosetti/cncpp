/*
 _____ __  __ _____
|  ___|  \/  |_   _|
| |_  | |\/| | | |
|  _| | |  | | | |
|_|   |_|  |_| |_|

FMT library
Author: Davide Stocco, 2025
*/

// STL library headers
#include <iostream>
#include <locale>

// FMT library header
#include <fmt/core.h>

// Basic usage: String Formatting
int main() {
  int age = 25;
  std::string name = "Alice";

  // Basic string formatting using placeholders
  std::string formatted_str = fmt::format("My name is {} and I am {} years old.", name, age);
  std::cout << formatted_str << std::endl;

  // Using positional arguments
  std::cout << fmt::format("{0} is {1} years old, and {0} is a software engineer.", name, age) << std::endl;

  // Using named arguments for clarity
  std::cout << fmt::format("{name} is {age} years old.", fmt::arg("name", name), fmt::arg("age", age)) << std::endl;

  // Formatting numbers with precision
  double pi = 3.14159;
  std::cout << fmt::format("Pi with 2 decimal places: {:.2f}", pi) << std::endl;

  // Width and alignment formatting
  int num = 42;
  std::cout << fmt::format("Right-aligned: {:>5}", num) << std::endl;
  std::cout << fmt::format("Left-aligned: {:<5}", num) << std::endl;

  // Formatting with colors (terminal output)
  std::string message = "Hello, World!";
  std::cout << fmt::format("\033[1;31m{}\033[0m", message) << std::endl; // Red text

  // Formatting numbers as hexadecimal
  std::cout << fmt::format("Hexadecimal: {0:x}", num) << std::endl;

  // Formatting floating-point numbers with precision
  std::cout << fmt::format("Pi with 4 decimal places: {:.4f}", pi) << std::endl;

  // Exception handling for format errors
  try {
    std::cout << fmt::format("This is an invalid format: {}", "missing closing brace") << std::endl;
  } catch (const fmt::format_error& e) {
    std::cout << "Caught format error: " << e.what() << std::endl;
  }

  /* FIXME:
  // Custom formatting for user-defined types
  struct Point {
    int x, y;
  };

  template<>
  struct fmt::formatter<Point> : fmt::formatter<std::string> {
    template <typename FormatContext>
    auto format(const Point& p, FormatContext& ctx) {
      return fmt::format_to(ctx.out(), "Point({}, {})", p.x, p.y);
    }
  };

  Point p = {3, 4};
  std::cout << fmt::format("The point is: {}", p) << std::endl;
  */

  return 0;
}
