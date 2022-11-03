add_library(map)


target_link_libraries(map
 PUBLIC
   lodepng
 )

target_include_directories(map
  PUBLIC
   ${PROJECT_SOURCE_DIR}/load_png
 )

target_sources(map
  PUBLIC
    map.h
  PRIVATE
    map.cpp
  )
