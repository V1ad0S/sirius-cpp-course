add_executable(main main.cpp)

add_subdirectory(linkedlist)

target_link_libraries(main linkedlist)
