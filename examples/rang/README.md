# `rang` library

[`rang`](https://github.com/agauniyal/rang) is a simple `C++` library that allows you to style and color your terminal output. It is lightweight and easy to use for enhancing text output with colors, styles, and effects.

## Basic usage

The `rang` library uses a simple and intuitive syntax to apply colors and styles to your terminal output.

To include the `rang` library in your code, you need to include the `rang.hpp` header file.

```cpp
#include <iostream>
#include "rang.hpp"
```

### Coloring text

`rang` provides predefined color codes for both foreground and background text. Here's an example of how to use them:

```cpp
// using foreground colors
std::cout << rang::fg::red << "This is red text!" << rang::style::reset << std::endl;
std::cout << rang::fg::green << "This is green text!" << rang::style::reset << std::endl;

// using background colors
std::cout << rang::bg::yellow << "This is text with yellow background!" << rang::style::reset << std::endl;
```

### Using styles

`rang` also supports several styles such as bold, underlined, and italicized text.

```cpp
// bold text
std::cout << rang::style::bold << "This is bold text!" << rang::style::reset << std::endl;

// underlined text
std::cout << rang::style::underline << "This is underlined text!" << rang::style::reset << std::endl;

// combining styles and colors
std::cout << rang::fg::cyan << rang::style::bold << "This is bold cyan text!" << rang::style::reset << std::endl;
```

### Combining colors and styles

You can easily combine multiple styles and colors in a single line of text.

```cpp
// combine color and style
std::cout << rang::fg::magenta << rang::style::bold << "This is bold magenta text!" << rang::style::reset << std::endl;
std::cout << rang::fg::blue << rang::style::italic << "This is italic blue text!" << rang::style::reset << std::endl;
```

### Resetting styles

Each time you apply a color or style, you should call the `reset` style to revert the changes. The `rang::style::reset` resets all styles and colors back to the default terminal settings.

```cpp
// apply color and reset style
std::cout << rang::fg::red << "This text is red" << rang::style::reset << std::endl;
std::cout << "This is normal text again.\n";
```

### Supported Colors and Styles

The `rang` library provides a wide range of colors and styles:

**Colors**:

Foreground `rang::fg`:
- `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, and `white`.

Background `rang::bg`:
- `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, and `white`.

**Styles `rang::style`**

- `reset` resets all styles
- `bold` makes the text bold
- `underline` underlines the text
- `italic` makes the text italicized (may not be supported by all terminals)

## Advanced usage

### Multi-line output

You can apply colors and styles to multiple lines easily.

```cpp
std::cout << rang::fg::yellow << "Line 1: Yellow Text!" << rang::style::reset << std::endl;
std::cout << rang::fg::green << "Line 2: Green Text!" << rang::style::reset << std::endl;
std::cout << rang::fg::blue << "Line 3: Blue Text!" << rang::style::reset << std::endl;
```
