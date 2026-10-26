cmake_minimum_required(VERSION 3.20)
set(VERSION "0.0.1")
project(CNCpp VERSION ${VERSION} LANGUAGES CXX)

#   ____       _
#  / ___|  ___| |_  __   ____ _ _ __ ___
#  \___ \ / _ \ __| \ \ / / _` | '__/ __|
#   ___) |  __/ |_   \ V / (_| | |  \__ \
#  |____/ \___|\__|   \_/ \__,_|_|  |___/

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(BUILD_FLAG "RELEASE_BUILD")
else()
  set(BUILD_FLAG "DEBUG_BUILD")
  add_compile_definitions(DEBUG=1)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MAIN_DIR ${SRC_DIR}/main)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/usr CACHE PATH "Install path prefix" FORCE)

include(CTest)

message(STATUS "SRC_DIR: ${SRC_DIR}")
message(STATUS "MAIN_DIR: ${MAIN_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*/_.*\\.cpp$")


#   _____         _   ____            _
#  |___ / _ __ __| | |  _ \ __ _ _ __| |_ _   _
#    |_ \| '__/ _` | | |_) / _` | '__| __| | | |
#   ___) | | | (_| | |  __/ (_| | |  | |_| |_| |
#  |____/|_|  \__,_| |_|   \__,_|_|   \__|\__, |
#                                         |___/

include(FetchContent)

message(STATUS "Fetching dependencies...")

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(rang
  GIT_REPOSITORY https://github.com/agauniyal/rang.git
  GIT_TAG        v3.2
  GIT_SHALLOW    TRUE
)

FetchContent_Declare(toml
  GIT_REPOSITORY https://github.com/pbosetti/tomlplusplus.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)

FetchContent_Declare(keystroker
  GIT_REPOSITORY https://github.com/pbosetti/keystroker.git
  GIT_SHALLOW    TRUE
  GIT_TAG HEAD
)

FetchContent_MakeAvailable(fmt rang json toml keystroker)

if(BUILD_TESTING)
  FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

message(STATUS "Dependencies fetched.")

include_directories(
  ${fmt_SOURCE_DIR}/include
  ${rang_SOURCE_DIR}/include
  ${json_SOURCE_DIR}/include
  ${toml_SOURCE_DIR}/include
  ${keystroker_SOURCE_DIR}/include
  # /opt/homebrew/include
)

if(WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE
                          _USE_MATH_DEFINES NOMINMAX)
  # Make MSVC report the actual C++ standard in __cplusplus
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)
elseif(APPLE)
  add_compile_options(-Wno-deprecated-literal-operator)
endif()

# find_package(Mads REQUIRED)
# message(STATUS "Found Mads: ${Mads_DIR}")

#    ____             __ _                          _    __ _ _
#   / ___|___  _ __  / _(_) __ _ _   _ _ __ ___  __| |  / _(_) | ___  ___
#  | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _ \/ _` | | |_| | |/ _ \/ __|
#  | |__| (_) | | | |  _| | (_| | |_| | | |  __/ (_| | |  _| | |  __/\__ \
#   \____\___/|_| |_|_| |_|\__, |\__,_|_|  \___|\__,_| |_| |_|_|\___||___/
#                          |___/

# configure_file(
#   ${SRC_DIR}/defines.hpp.in
#   ${SRC_DIR}/defines.hpp
# )

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples)

#  ____                                        _        _   _
# |  _ \  ___   ___ _   _ _ __ ___   ___ _ __ | |_ __ _| |_(_) ___  _ __
# | | | |/ _ \ / __| | | | '_ ` _ \ / _ \ '_ \| __/ _` | __| |/ _ \| '_ \
# | |_| | (_) | (__| |_| | | | | | |  __/ | | | || (_| | |_| | (_) | | | |
# |____/ \___/ \___|\__,_|_| |_| |_|\___|_| |_|\__\__,_|\__|_|\___/|_| |_|

find_package(Doxygen)

if(DOXYGEN_FOUND)
  set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
  set(DOXYGEN_PROJECT_VERSION "${PROJECT_VERSION}")
  set(DOXYGEN_INPUT_DIR "${SRC_DIR}")
  set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/doc")
  set(DOXYGEN_FILE_PATTERNS "*.hpp")
  set(DOXYGEN_RECURSIVE YES)

  configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/doc/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
  )

  add_custom_target(doc
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
else()
  message(STATUS "Doxygen not found: 'doc' target will not be available")
endif()

#   _____                    _
#  |_   _|_ _ _ __ __ _  ___| |_ ___
#    | |/ _` | '__/ _` |/ _ \ __/ __|
#    | | (_| | | | (_| |  __/ |_\__ \
#    |_|\__,_|_|  \__, |\___|\__|___/
#                 |___/

# main Library
file(GLOB HEADERS ${SRC_DIR}/*.hpp)

add_library(cncpp STATIC ${SRC_FILES})
target_include_directories(cncpp PUBLIC ${SRC_DIR})
target_link_libraries(cncpp PUBLIC fmt::fmt)

add_executable(point_test ${SRC_DIR}/point.cpp)
target_compile_definitions(point_test PRIVATE CNCPP_TEST_POINT)
target_link_libraries(point_test PRIVATE fmt::fmt)

#   _____         _
#  |_   _|__  ___| |_ ___
#    | |/ _ \/ __| __/ __|
#    | |  __/\__ \ |_\__ \
#    |_|\___||___/\__|___/

if(BUILD_TESTING)
  include(Catch)
  file(GLOB TEST_FILES ${CMAKE_CURRENT_LIST_DIR}/tests/test_*.cpp)
  foreach(test_src ${TEST_FILES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE cncpp Catch2::Catch2WithMain)
    catch_discover_tests(${test_name})
  endforeach()
endif()



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
