add_executable(main main.cpp)

add_subdirectory(parser)
add_subdirectory(polygon)

target_link_libraries(main
  PRIVATE
    polygon
    parser
  )
