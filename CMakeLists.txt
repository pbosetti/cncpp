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
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(MAIN_DIR ${SRC_DIR}/main)

message(STATUS "SRC_DIR: ${SRC_DIR}")
message(STATUS "MAIN_DIR: ${MAIN_DIR}")
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)


#   _____         _   ____            _         
#  |___ / _ __ __| | |  _ \ __ _ _ __| |_ _   _ 
#    |_ \| '__/ _` | | |_) / _` | '__| __| | | |
#   ___) | | | (_| | |  __/ (_| | |  | |_| |_| |
#  |____/|_|  \__,_| |_|   \__,_|_|   \__|\__, |
#                                         |___/ 
                    
include(FetchContent)

message(STATUS "Fetching dependencies...")

FetchContent_Declare(tomlplusplus
  GIT_REPOSITORY https://github.com/pbosetti/tomlplusplus.git
  GIT_TAG        050c7a0
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

FetchContent_MakeAvailable(tomlplusplus fmt rang)

message(STATUS "Dependencies fetched.")

include_directories(
  ${tomlplusplus_SOURCE_DIR}/include
  ${fmt_SOURCE_DIR}/include
  ${rang_SOURCE_DIR}/include
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

#   _____                    _       
#  |_   _|_ _ _ __ __ _  ___| |_ ___ 
#    | |/ _` | '__/ _` |/ _ \ __/ __|
#    | | (_| | | | (_| |  __/ |_\__ \
#    |_|\__,_|_|  \__, |\___|\__|___/
#                 |___/              

# main Library
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)
add_library(cncpp STATIC ${SRC_FILES})
target_link_libraries(cncpp PRIVATE fmt::fmt)

# Test executables
add_executable(point_test ${SRC_DIR}/point.cpp)
target_link_libraries(point_test PRIVATE cncpp)
target_compile_definitions(point_test PRIVATE POINT_MAIN)

add_executable(block_test ${SRC_DIR}/block.cpp)
target_link_libraries(block_test PRIVATE cncpp)
target_compile_definitions(block_test PRIVATE BLOCK_MAIN)

add_executable(program_test ${SRC_DIR}/program.cpp)
target_link_libraries(program_test PRIVATE cncpp)
target_compile_definitions(program_test PRIVATE PROGRAM_MAIN)

