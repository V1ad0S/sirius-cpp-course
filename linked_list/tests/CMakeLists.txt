add_executable(list_test list_tester.cpp)

target_include_directories(list_test
  PRIVATE
    ${PROJECT_SOURCE_DIR}/src/linkedlist
  )
target_link_libraries(list_test linkedlist)

add_test(
  NAME test
  COMMAND list_test
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )


