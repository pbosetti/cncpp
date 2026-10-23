if(BUILD_TESTING)
  add_executable(point_unit_tests
    ${CMAKE_CURRENT_LIST_DIR}/point_unit_tests.cpp
  )
  target_link_libraries(point_unit_tests PRIVATE cncpp)
  add_test(NAME point_unit_tests COMMAND point_unit_tests)

  add_executable(block_unit_tests
    ${CMAKE_CURRENT_LIST_DIR}/block_unit_tests.cpp
  )
  target_link_libraries(block_unit_tests PRIVATE cncpp)
  add_test(NAME block_unit_tests COMMAND block_unit_tests)

  add_executable(machine_unit_tests
    ${CMAKE_CURRENT_LIST_DIR}/machine_unit_tests.cpp
  )
  target_link_libraries(machine_unit_tests PRIVATE cncpp)
  target_compile_definitions(machine_unit_tests PRIVATE
    CNCPP_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
  add_test(NAME machine_unit_tests COMMAND machine_unit_tests)
endif()
