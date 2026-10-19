cmake_minimum_required(VERSION 3.20)
project(Lesson4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

add_library(thermostat STATIC src/thermostat.cpp)
target_include_directories(thermostat PUBLIC src)

add_executable(lists src/main/01_lists.cpp)
target_link_libraries(lists PRIVATE thermostat)

add_executable(algorithms src/main/02_algorithms.cpp)
target_link_libraries(algorithms PRIVATE thermostat)

add_executable(maps src/main/03_maps.cpp)
target_link_libraries(maps PRIVATE thermostat)

# cmake -B build
# cmake --build build
