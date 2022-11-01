add_executable(main main.cpp)

add_subdirectory(rbtree)

target_link_libraries(main rbtree)
