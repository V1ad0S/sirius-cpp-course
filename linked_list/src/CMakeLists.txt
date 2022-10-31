add_executable(main main.cpp)

add_subdirectory(linkedlist)

target_link_libraries(main linkedlist)
target_sources(main
  PUBLIC
    exceptions.h
  PRIVATE
    exceptions.cpp
  )
