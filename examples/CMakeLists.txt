#   __  __
#  |  \/  | __ _  ___ _ __ ___  ___
#  | |\/| |/ _` |/ __| '__/ _ \/ __|
#  | |  | | (_| | (__| | | (_) \__ \
#  |_|  |_|\__,_|\___|_|  \___/|___/
#

macro(create_example target)
  set(multiValueArgs SRCS LIBS DEFS)
  cmake_parse_arguments(ARG "" "" "${multiValueArgs}" ${ARGN})
  list(TRANSFORM ARG_SRCS PREPEND "${CMAKE_CURRENT_LIST_DIR}/${target}/")
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

create_example(basics SRCS basics.cpp Car.cpp)
create_example(inheritance SRCS inheritance.cpp)
create_example(fmt SRCS fmt.cpp LIBS fmt::fmt)
create_example(rang SRCS rang.cpp LIBS fmt::fmt)
create_example(stl_vector SRCS stl_vector.cpp)
create_example(stl_list SRCS stl_list.cpp)
create_example(stl_map SRCS stl_map.cpp)
create_example(json SRCS json.cpp)
create_example(yaml SRCS yaml.cpp LIBS yaml-cpp::yaml-cpp)
create_example(mqtt_sub SRCS mqtt_sub.cpp LIBS mosquittopp_static mosquitto_static)
add_dependencies(ex_mqtt_sub mosquitto)