cmake_minimum_required(VERSION 3.20)

project(CNCppPython VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 11.0.2
  GIT_SHALLOW TRUE
)

FetchContent_Declare(toml
  GIT_REPOSITORY https://github.com/pbosetti/tomlplusplus.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(fmt json toml)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG QUIET)
if(NOT pybind11_FOUND)
  FetchContent_Declare(pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v3.0.4
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(pybind11)
endif()

set(CNCPP_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(CNCPP_SOURCES
  ${CNCPP_ROOT}/src/block.cpp
  ${CNCPP_ROOT}/src/machine.cpp
  ${CNCPP_ROOT}/src/point.cpp
  ${CNCPP_ROOT}/src/program.cpp
)

pybind11_add_module(_cncpp
  src/cncpp_python.cpp
  ${CNCPP_SOURCES}
)

target_include_directories(_cncpp PRIVATE
  ${CNCPP_ROOT}/src
  ${json_SOURCE_DIR}/include
  ${toml_SOURCE_DIR}/include
)

target_link_libraries(_cncpp PRIVATE fmt::fmt)
target_compile_features(_cncpp PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(_cncpp PRIVATE -Wno-deprecated-literal-operator)
endif()

set_target_properties(_cncpp PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cncpp
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cncpp
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cncpp
)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/cncpp/__init__.py
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cncpp)

install(TARGETS _cncpp
  LIBRARY DESTINATION cncpp
  RUNTIME DESTINATION cncpp
)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/cncpp/__init__.py DESTINATION cncpp)
