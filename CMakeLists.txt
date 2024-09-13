cmake_minimum_required(VERSION 3.20)
set(VERSION "0.0.1")
project(CNCpp VERSION ${VERSION} LANGUAGES CXX)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(BUILD_FLAG "RELEASE_BUILD")
else()
  set(BUILD_FLAG "DEBUG_BUILD")
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
target_link_libraries(cncpp PRIVATE fmt::fmt)

add_executable(point_test ${SRC_DIR}/point.cpp)
target_link_libraries(point_test PRIVATE cncpp)
target_compile_definitions(point_test PRIVATE POINT_MAIN)

add_executable(block_test ${SRC_DIR}/block.cpp)
target_link_libraries(block_test PRIVATE cncpp)
target_compile_definitions(block_test PRIVATE BLOCK_MAIN)

add_executable(program_test ${SRC_DIR}/program.cpp)
target_link_libraries(program_test PRIVATE cncpp)
target_compile_definitions(program_test PRIVATE PROGRAM_MAIN)

