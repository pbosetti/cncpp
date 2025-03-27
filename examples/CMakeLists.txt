#   __  __
#  |  \/  | __ _  ___ _ __ ___  ___
#  | |\/| |/ _` |/ __| '__/ _ \/ __|
#  | |  | | (_| | (__| | | (_) \__ \
#  |_|  |_|\__,_|\___|_|  \___/|___/
#

macro(create_example target)
  set(multiValueArgs SRCS LIBS DEFS DEPENDS)
  cmake_parse_arguments(ARG "" "" "${multiValueArgs}" ${ARGN})
  list(TRANSFORM ARG_SRCS PREPEND "${CMAKE_CURRENT_LIST_DIR}/${target}/")
  message(STATUS "Creating example: ${target}")
  message(STATUS "  Sources: ${ARG_SRCS}")
  add_executable(ex_${target} ${ARG_SRCS})
  list(APPEND TARGET_LIST ex_${target})
  target_link_libraries(ex_${target} ${ARG_LIBS})
  target_compile_definitions(ex_${target} PRIVATE ${ARG_DEFS})
  if(ARG_DEPENDS)
    add_dependencies(ex_${target} ${ARG_DEPENDS})
  endif()
  set_target_properties(ex_${target} PROPERTIES OUTPUT_NAME ${target})
endmacro()

#   _____                           _        _                       _
#  | ____|_  ____ _ _ __ ___  _ __ | | ___  | |_ __ _ _ __ __ _  ___| |_ ___
#  |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \ | __/ _` | '__/ _` |/ _ \ __/ __|
#  | |___ >  < (_| | | | | | | |_) | |  __/ | || (_| | | | (_| |  __/ |_\__ \
#  |_____/_/\_\__,_|_| |_| |_| .__/|_|\___|  \__\__,_|_|  \__, |\___|\__|___/
#                            |_|                          |___/

create_example(lesson1 SRCS test.cpp)
create_example(lesson2 SRCS test.cpp)
create_example(lesson3 SRCS test.cpp)
