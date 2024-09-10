cmake_minimum_required(VERSION 3.20)
set(VERSION "0.0.1")
project(CNCpp VERSION ${VERSION} LANGUAGES CXX)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr/local")
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}") 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
message(STATUS "SRC_DIR: ${SRC_DIR}")

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

configure_file(
  ${SRC_DIR}/cncpp.hpp.in 
  ${SRC_DIR}/cncpp.hpp
)


include(FetchContent)
FetchContent_Declare(tomlplusplus
    GIT_REPOSITORY https://github.com/pbosetti/tomlplusplus.git
    GIT_TAG        050c7a0
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        11.0.2
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(tomlplusplus fmt)

include_directories(
  ${tomlplusplus_SOURCE_DIR}/include
  ${fmt_SOURCE_DIR}/include
)

file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)



add_library(cncpp STATIC ${SRC_FILES})

add_executable(point_test ${SRC_DIR}/point.cpp)
target_link_libraries(point_test PRIVATE fmt::fmt)
target_compile_definitions(point_test PRIVATE POINT_MAIN)