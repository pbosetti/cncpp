/*
 ____
|  _ \ __ _ _ __   __ _
| |_) / _` | '_ \ / _` |
|  _ < (_| | | | | (_| |
|_| \_\__,_|_| |_|\__, |
                  |___/
JSON library
Author: Davide Stocco, 2025
*/

// STL library headers
#include <iostream>

// Rang library header
#include <rang.hpp>

// FMT library header
#include <fmt/core.h>

int main() {
  // Basic Usage - Coloring Text

  // Using foreground colors
  std::cout << rang::fg::red << "This is red text!" << rang::style::reset << std::endl;
  std::cout << rang::fg::green << "This is green text!" << rang::style::reset << std::endl;

  // Using background colors
  std::cout << rang::bg::yellow << "This is text with yellow background!" << rang::style::reset << std::endl;

  // Styling Text

  // Applying bold text style
  std::cout << rang::style::bold << "This is bold text!" << rang::style::reset << std::endl;

  // Applying underlined text style
  std::cout << rang::style::underline << "This is underlined text!" << rang::style::reset << std::endl;

  // Combining styles and colors
  std::cout << rang::fg::cyan << rang::style::bold << "This is bold cyan text!" << rang::style::reset << std::endl;

  // Combining color and Style
  std::cout << rang::fg::magenta << rang::style::bold << "This is bold magenta text!" << rang::style::reset << std::endl;
  std::cout << rang::fg::blue << rang::style::italic << "This is italic blue text!" << rang::style::reset << std::endl;

  // Resetting Styles

  // Apply color and reset the style
  std::cout << rang::fg::red << "This text is red" << rang::style::reset << std::endl;
  std::cout << "This is normal text again.\n";

  // Advanced Usage - Multi-line Output
  std::cout << rang::fg::yellow << "Line 1: Yellow Text!" << rang::style::reset << std::endl;
  std::cout << rang::fg::green << "Line 2: Green Text!" << rang::style::reset << std::endl;
  std::cout << rang::fg::blue << "Line 3: Blue Text!" << rang::style::reset << std::endl;

  // Combining fmt and rang libraries
  std::cout << rang::fg::cyan << fmt::format("This is cyan text using fmt and rang!\n") << rang::style::reset;

  return 0;
}
