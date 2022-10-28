add_executable(poly_test poly_tester.cpp)
target_include_directories(poly_test
  PRIVATE ${PROJECT_SOURCE_DIR}/src/polygon
  )
target_link_libraries(poly_test polygon)

add_test(
    NAME poly_test.outside_points
  COMMAND poly_test 0
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_test(
    NAME poly_test.inside_points
  COMMAND poly_test 1
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_test(
    NAME poly_test.edge_points
  COMMAND poly_test 2
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

