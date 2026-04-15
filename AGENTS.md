# Purpose
This repo contains the implementation of a Computer Numerical Control (CNC) kernel in C++, developed as a training project within a master course in Digital manufacturing for Mechatronics Engineering students. The CNC kernel is designed to interpret G-code commands and control the movement of a CNC machine, enabling precise machining operations.

# Repo structure

- `src`: contains library files
- `src/main`: contains the main executable file(s)
- `examples`: contains C++ introductory examples
- `goodies`: containse utlities and scripts for development and testing
- `build`: out-of-source build directory (not versioned)

# Code styling guide

* Use CamelCase for class names and namespaces
* Use snake_case for variable names and function names
* Put private members at the end of a class
* Use `_var_name` with leading underscore for private members
* Use LLVM standard for formatting conventions with 2 spaces for indentation and no tabs
* In class headers, organize public methods in three sections (within each section, order them alphabetically, separate section with a UPPERCASE COMMENT LABEL):
  1. Public static functions
  2. Constructors and destructors
  3. Public member functions
  4. Accessors and mutators
* Use the same order in the implementation files, using a Figlet style comment to separate sections
* Doxygen-style comments for public members in the headers files, with a brief description of the function, its parameters, and its return value (if applicable). For private members, use regular comments to explain their purpose and usage.
* Use `.hpp` extension for header files and `.cpp` extension for implementation files.

# Project layout and design
The project is organized into a library and at least one executable. The library contains the core functionality of the CNC kernel, including classes and functions for interpreting G-code commands, controlling machine movements, and managing the state of the CNC machine.

Create a static library target for all files in `src`, and an executable target for each source file in `src/main`. Link the executable targets against the static library target to ensure that they can use the functionality provided by the library.  This approach promotes modularity and reusability of the code, allowing the main executable to focus on high-level logic while the library encapsulates the core functionality of the CNC kernel.

Ensure that there is a main headerfile `cncpp.hpp` in the `src` directory that includes all the necessary headers for the library, and that the executable files in `src/main` include this main header to access the library's functionality.

# Target compilers and operating systems
Assume thet as a compiler we use clang and as operating systems we support: 

* Linux
* MacOS

# Build system
Use CMake as the build system. Prefer modern CMake practices, such as using `target_include_directories` and `target_link_libraries` to specify dependencies, and avoid using global variables for include directories and libraries. Use `CMAKE_EXPORT_COMPILE_COMMANDS` to generate a `compile_commands.json` file for better integration with code editors and tools.

Ninja is preferred as the build tool for its speed and efficiency, but the build system should also support Makefiles for compatibility with environments where Ninja is not available.

We assume C++20 as the minimum standard for the project, and we should ensure that the CMake configuration enforces this requirement.

Preferred configure/build commands:

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build -j5
```

# Testing

Write unit tests for each class in `src`, with minimum complexity end use of external tools.