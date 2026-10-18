cmake_minimum_required(VERSION 3.20)
project(Lesson3 LANGUAGES CXX)

# C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Adding more compilation warnings
add_compile_options(-Wall -Wextra)

# Common library for every executable
add_library(thermostat STATIC src/thermostat.cpp)
target_include_directories(thermostat PUBLIC src)

## Executables here
add_executable(references src/main/01_references.cpp)
target_link_libraries(references PRIVATE thermostat)

add_executable(templates src/main/02_templates.cpp)
target_link_libraries(templates PRIVATE thermostat)

add_executable(vectors src/main/03_vectors.cpp)
target_link_libraries(vectors PRIVATE thermostat)

add_executable(operators src/main/04_operators.cpp)
target_link_libraries(operators PRIVATE thermostat)

add_executable(lambdas src/main/05_lambdas.cpp)
target_link_libraries(lambdas PRIVATE thermostat)
