add_library(polygon)

target_sources(polygon
  PUBLIC
    exceptions.h
    poly.h
  PRIVATE
    exceptions.cpp
    poly.cpp
  )

target_include_directories(polygon
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
