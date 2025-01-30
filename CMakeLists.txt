cmake_minimum_required(VERSION 3.20)
set(VERSION "0.0.1")
project(CNCpp VERSION ${VERSION} LANGUAGES CXX)

#   ____       _                         
#  / ___|  ___| |_  __   ____ _ _ __ ___ 
#  \___ \ / _ \ __| \ \ / / _` | '__/ __|
#   ___) |  __/ |_   \ V / (_| | |  \__ \
#  |____/ \___|\__|   \_/ \__,_|_|  |___/
                                       
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(BUILD_FLAG "RELEASE_BUILD")
else()
  set(BUILD_FLAG "DEBUG_BUILD")
  add_compile_definitions(DEBUG=1)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(MAIN_DIR ${SRC_DIR}/main)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/usr CACHE PATH "Install path prefix" FORCE)

message(STATUS "SRC_DIR: ${SRC_DIR}")
message(STATUS "MAIN_DIR: ${MAIN_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)


#   _____         _   ____            _         
#  |___ / _ __ __| | |  _ \ __ _ _ __| |_ _   _ 
#    |_ \| '__/ _` | | |_) / _` | '__| __| | | |
#   ___) | | | (_| | |  __/ (_| | |  | |_| |_| |
#  |____/|_|  \__,_| |_|   \__,_|_|   \__|\__, |
#                                         |___/ 
                    
include(FetchContent)

message(STATUS "Fetching dependencies...")

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        HEAD
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        11.0.2
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(rang
  GIT_REPOSITORY https://github.com/agauniyal/rang.git
  GIT_TAG        v3.2
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_SHALLOW    TRUE
  GIT_TAG HEAD
)

FetchContent_Declare(keystroker
  GIT_REPOSITORY https://github.com/pbosetti/keystroker.git
  GIT_SHALLOW    TRUE
  GIT_TAG HEAD
)

set(DOCUMENTATION OFF)
set(INC_BRIDGE_SUPPORT OFF)
set(WITH_APPS OFF)
set(WITH_CLIENTS OFF)
set(WITH_PIC ON)
set(WITH_STATIC_LIBRARIES ON)
set(WITH_TLS OFF)
set(WITH_LIB_CPP ON)
FetchContent_Declare(mosquitto
  GIT_REPOSITORY https://github.com/eclipse-mosquitto/mosquitto.git
  GIT_SHALLOW    TRUE
  GIT_TAG v2.0.20
)

FetchContent_MakeAvailable(fmt rang json yaml-cpp keystroker mosquitto)

message(STATUS "Dependencies fetched.")

include_directories(
  ${fmt_SOURCE_DIR}/include
  ${rang_SOURCE_DIR}/include
  ${json_SOURCE_DIR}/include
  ${yaml-cpp_SOURCE_DIR}/include
  ${keystroker_SOURCE_DIR}/include
  ${mosquitto_SOURCE_DIR}/include
  ${mosquitto_SOURCE_DIR}/lib/cpp
  # /opt/homebrew/include
)

link_directories(
  ${mosquitto_BINARY_DIR}/lib
  ${mosquitto_BINARY_DIR}/lib/cpp
)

#    ____             __ _                          _    __ _ _           
#   / ___|___  _ __  / _(_) __ _ _   _ _ __ ___  __| |  / _(_) | ___  ___ 
#  | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _ \/ _` | | |_| | |/ _ \/ __|
#  | |__| (_) | | | |  _| | (_| | |_| | | |  __/ (_| | |  _| | |  __/\__ \
#   \____\___/|_| |_|_| |_|\__, |\__,_|_|  \___|\__,_| |_| |_|_|\___||___/
#                          |___/                                          

configure_file(
  ${SRC_DIR}/defines.hpp.in 
  ${SRC_DIR}/defines.hpp
)


#   __  __                          
#  |  \/  | __ _  ___ _ __ ___  ___ 
#  | |\/| |/ _` |/ __| '__/ _ \/ __|
#  | |  | | (_| | (__| | | (_) \__ \
#  |_|  |_|\__,_|\___|_|  \___/|___/
#                                  
macro(create_example target)
  set(multiValueArgs SRCS LIBS DEFS)
  cmake_parse_arguments(ARG "" "" "${multiValueArgs}" ${ARGN})
  list(TRANSFORM ARG_SRCS PREPEND "${CMAKE_CURRENT_LIST_DIR}/examples/${target}/")
  message(STATUS "Creating example: ${target}")
  message(STATUS "  Sources: ${ARG_SRCS}")
  add_executable(ex_${target} ${ARG_SRCS})
  list(APPEND TARGET_LIST ex_${target})
  target_link_libraries(ex_${target} ${ARG_LIBS})
  target_compile_definitions(ex_${target} PRIVATE ${ARG_DEFS})
endmacro()


#   _____                           _        _                       _       
#  | ____|_  ____ _ _ __ ___  _ __ | | ___  | |_ __ _ _ __ __ _  ___| |_ ___ 
#  |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \ | __/ _` | '__/ _` |/ _ \ __/ __|
#  | |___ >  < (_| | | | | | | |_) | |  __/ | || (_| | | | (_| |  __/ |_\__ \
#  |_____/_/\_\__,_|_| |_| |_| .__/|_|\___|  \__\__,_|_|  \__, |\___|\__|___/
#                            |_|                          |___/              

create_example(basics SRCS basics.cpp myobject.cpp)
create_example(rang SRCS rang.cpp)
create_example(mqtt_sub SRCS mqtt_sub.cpp LIBS mosquittopp_static mosquitto_static)


#   _____                    _       
#  |_   _|_ _ _ __ __ _  ___| |_ ___ 
#    | |/ _` | '__/ _` |/ _ \ __/ __|
#    | | (_| | | | (_| |  __/ |_\__ \
#    |_|\__,_|_|  \__, |\___|\__|___/
#                 |___/              

# main Library
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB HEADERS ${SRC_DIR}/*.hpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*fsm.*")
add_library(cncpp_lib STATIC ${SRC_FILES})
target_link_libraries(cncpp_lib PRIVATE fmt::fmt mosquittopp_static mosquitto_static yaml-cpp::yaml-cpp keystroker)
set_target_properties(cncpp_lib PROPERTIES PUBLIC_HEADER "${HEADERS}")
list(APPEND TARGET_LIST cncpp_lib)

# Test executables
add_executable(point_test ${SRC_DIR}/point.cpp)
target_link_libraries(point_test PRIVATE cncpp_lib)
target_compile_definitions(point_test PRIVATE POINT_MAIN)
list(APPEND TARGET_LIST point_test)

add_executable(block_test ${SRC_DIR}/block.cpp)
target_link_libraries(block_test PRIVATE cncpp_lib)
target_compile_definitions(block_test PRIVATE BLOCK_MAIN)
list(APPEND TARGET_LIST block_test)

add_executable(program_test ${SRC_DIR}/program.cpp)
target_link_libraries(program_test PRIVATE cncpp_lib)
target_compile_definitions(program_test PRIVATE PROGRAM_MAIN)
list(APPEND TARGET_LIST program_test)

add_executable(machine_test ${SRC_DIR}/machine.cpp)
target_link_libraries(machine_test PRIVATE cncpp_lib)
target_compile_definitions(machine_test PRIVATE MACHINE_MAIN)
list(APPEND TARGET_LIST machine_test)

# Main exec
add_executable(cncpp ${MAIN_DIR}/cncpp.cpp)
target_link_libraries(cncpp cncpp_lib)


#   ___           _        _ _ 
#  |_ _|_ __  ___| |_ __ _| | |
#   | || '_ \/ __| __/ _` | | |
#   | || | | \__ \ || (_| | | |
#  |___|_| |_|___/\__\__,_|_|_|
#  

install(TARGETS ${TARGET_LIST}
  BUNDLE DESTINATION bin
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
)