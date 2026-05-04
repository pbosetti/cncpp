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
endif()
