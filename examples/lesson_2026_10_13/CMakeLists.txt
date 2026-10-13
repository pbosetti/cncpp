cmake_minimum_required(VERSION 3.20)

add_library(thermostat src/thermostat.cpp)
target_include_directories(thermostat PUBLIC src/)

add_executable(encapsulation src/main/encapsulation.cpp)
target_link_libraries(encapsulation PRIVATE thermostat)

add_executable(log src/main/log.cpp)
target_link_libraries(log PRIVATE thermostat)

# cmake -B build
# cmake --build build