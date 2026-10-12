# These are the two commands you will have to remember
# cmake -B build # Configure the project
# cmake --build build # Build the project

# Each one of these lines will create a executable
# called as the first argument and with the
# sources files specified after the executable name
# in the first case:
# hello is the executalbe name
# example_1/hello.cpp is the only one source file

add_executable(hello example_1/hello.cpp)

add_executable(basic example_2/basic.cpp example_2/units.cpp)

add_executable(ex3 example_3/main.cpp)

add_executable(thermostat example_4/main.cpp example_4/thermostat.cpp)