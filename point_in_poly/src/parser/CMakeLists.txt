add_library(parser)

target_sources(parser
  PUBLIC
    parse_arguments.h
  PRIVATE
    parse_arguments.cpp
  )

target_include_directories(parser
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
