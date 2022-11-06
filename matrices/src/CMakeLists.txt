add_executable(main main.cpp)

add_subdirectory(matrix)

target_link_libraries(main matrix)
