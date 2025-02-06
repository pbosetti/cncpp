# `rang` library

[`rang`](https://github.com/agauniyal/rang) is a simple C++ library that allows you to style and color your terminal output. It is lightweight and easy to use for enhancing text output with colors, styles, and effects.

## Basic usage

The Rang library uses a simple and intuitive syntax to apply colors and styles to your terminal output.

### Coloring text

Rang provides predefined color codes for both foreground and background text. Here's an example of how to use them:

```cpp
#include <iostream>
#include "rang.hpp"

int main() {
    // using foreground colors
    std::cout << rang::fg::red << "This is red text!" << rang::style::reset << "\n";
    std::cout << rang::fg::green << "This is green text!" << rang::style::reset << "\n";

    // using background colors
    std::cout << rang::bg::yellow << "This is text with yellow background!" << rang::style::reset << "\n";

    return 0;
}
```

### Using styles

Rang also supports several styles such as bold, underlined, and italicized text.

```cpp
#include <iostream>
#include "rang.hpp"

int main() {
    // bold text
    std::cout << rang::style::bold << "This is bold text!" << rang::style::reset << "\n";

    // underlined text
    std::cout << rang::style::underline << "This is underlined text!" << rang::style::reset << "\n";

    // combining styles and colors
    std::cout << rang::fg::cyan << rang::style::bold << "This is bold cyan text!" << rang::style::reset << "\n";

    return 0;
}
```

### Combining colors and styles

You can easily combine multiple styles and colors in a single line of text.

```cpp
#include <iostream>
#include "rang.hpp"

int main() {
    // combine color and style
    std::cout << rang::fg::magenta << rang::style::bold << "This is bold magenta text!" << rang::style::reset << "\n";
    std::cout << rang::fg::blue << rang::style::italic << "This is italic blue text!" << rang::style::reset << "\n";

    return 0;
}
```

### Resetting styles

Each time you apply a color or style, you should call the `reset` style to revert the changes. The `rang::style::reset` resets all styles and colors back to the default terminal settings.

```cpp
#include <iostream>
#include "rang.hpp"

int main() {
    // apply color and reset style
    std::cout << rang::fg::red << "This text is red" << rang::style::reset << "\n";
    std::cout << "This is normal text again.\n";

    return 0;
}
```

### Supported Colors and Styles

The Rang library provides a wide range of colors and styles:

**Colors**:

- `rang::fg::black`, `rang::fg::red`, `rang::fg::green`, `rang::fg::yellow`, `rang::fg::blue`, `rang::fg::magenta`, `rang::fg::cyan`, `rang::fg::white`
- `rang::bg::black`, `rang::bg::red`, `rang::bg::green`, `rang::bg::yellow`, `rang::bg::blue`, `rang::bg::magenta`, `rang::bg::cyan`, `rang::bg::white`

**Styles**:

- `rang::style::reset`: Resets all styles
- `rang::style::bold`: Makes the text bold
- `rang::style::underline`: Underlines the text
- `rang::style::italic`: Makes the text italicized (may not be supported by all terminals)

## Advanced usage

### Multi-line output

You can apply colors and styles to multiple lines easily.

```cpp
#include <iostream>
#include "rang.hpp"

int main() {
    std::cout << rang::fg::yellow << "Line 1: Yellow Text!" << rang::style::reset << "\n";
    std::cout << rang::fg::green << "Line 2: Green Text!" << rang::style::reset << "\n";
    std::cout << rang::fg::blue << "Line 3: Blue Text!" << rang::style::reset << "\n";

    return 0;
}
```
