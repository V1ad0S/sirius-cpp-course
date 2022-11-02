add_executable(main main.cpp)

#add_subdirectory(dijkstra)
add_subdirectory(map)

target_link_libraries(main
  PRIVATE
    map
  )
