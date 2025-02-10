# `fmt` library

[fmt](https://github.com/fmtlib/fmt) is a modern formatting library for `C++` that provides a fast and flexible way to format strings. It provides similar functionality to Python's `f-string` but for `C++`.

## Basic usage

The `fmt` library provides a simple and modern interface for string formatting, similar to Python's f-strings.

To include the `fmt` library in your code, you need to include the `fmt/core.h` header file.

```cpp
#include <fmt/core.h>
```

### String formatting

The `fmt::format` function is used to format strings. You can specify placeholders using curly braces `{}` and pass the values to be inserted in those positions.

```cpp
int age = 25;
std::string name = "Alice";

// format string using fmt
std::string formatted_str = fmt::format("My name is {} and I am {} years old.", name, age);

std::cout << formatted_str << std::endl;
```
```
My name is Alice and I am 25 years old.
```

### Printing formatted strings

You can also directly print formatted strings using `fmt::print`. This will output the formatted string to the console.

```cpp
fmt::print("My name is {} and I am {} years old.", name, age);
```
```
My name is Alice and I am 25 years old.
```

### Using positional and named arguments

You can use positional to improve clarity and avoid mistakes with the order of arguments.

```cpp
std::cout << fmt::format("{0} is {1} years old, and {0} is a software engineer.", name, age) << std::endl;
```
```
Alice is 25 years old, and Alice is a software engineer.
```

Arguments can also be named for better readability.

```cpp
sd::cout << fmt::format("{name} is {age} years old.", fmt::arg("name", name), fmt::arg("age", age)) << std::endl;
```
```
Alice is 25 years old.
```

### Formatting numbers

You can format numbers with various specifications such as decimal precision, width, alignment, and more.

```cpp
double pi = 3.14159;

// format numbers with precision
std::cout << fmt::format("Pi with 2 decimal places: {:.2f}", pi) << std::endl;

// width and alignment
int num = 42;
std::cout << fmt::format("Right-aligned: {:>5}", num) << std::endl;
std::cout << fmt::format("Left-aligned: {:<5}", num) << std::endl;
```
```
Pi with 2 decimal places: 3.14
Right-aligned:    42
Left-aligned: 42
```

### Formatting with colors (terminal output)

You can also use the `fmt` library with terminal color codes by combining it with ANSI escape codes.

```cpp
std::string message = "Hello, World!";

// adding colors to the formatted text
std::cout << fmt::format("\033[1;31m{}\033[0m", message) << std::endl; // red text
```

This approach uses raw ANSI escape codes but works seamlessly with `fmt::format`.

### Formatting for specific types

You can use special format specifiers for specific types, such as dates, hexadecimal numbers, and more.

```cpp
int num = 255;
double pi = 3.14159;

// Formatting numbers in hexadecimal
std::cout << fmt::format("Hexadecimal: {0:x}", num) << std::endl;

// Formatting floating-point numbers
std::cout << fmt::format("Pi with 4 decimal places: {:.4f}", pi) << std::endl;
```
```
Hexadecimal: ff
Pi with 4 decimal places: 3.1416
```

### Exception handling with `fmt`

The `fmt` library allows handling exceptions when format errors occur. For example, an invalid format string or mismatched arguments will throw a `fmt::format_error`.

```cpp
try {
  std::cout << fmt::format("This is an invalid format: {}", "missing closing brace") << std::endl;
} catch (const fmt::format_error& e) {
  std::cout << "Caught format error: " << e.what() << std::endl;
}
```
```
Caught format error: format string contains too many arguments
```

## Advanced features

### Custom formatting

If needed, you can define your own custom formatters for user-defined types by specializing `fmt::formatter`.

```cpp
#include <fmt/core.h>
#include <iostream>

struct Point {
  std::string name;
  int x, y;
};

template <>
class fmt::formatter<Point> {
public:
  constexpr auto parse (format_parse_context& ctx) {return ctx.begin() }

  template <typename Context>
  constexpr auto format (Point const& point, Context& ctx) const {
    return format_to(ctx.out(), "{}({}, {})", point.name, point.x, point.y);
  }
};

int main() {
  Point p = {"P1", 3, 4};
  std::cout << fmt::format("The point is: {}", p) << std::endl;

  return 0;
}
```
```
The point is: P1(3, 4)
```
