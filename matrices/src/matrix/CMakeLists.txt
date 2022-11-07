add_library(matrix)

set_target_properties(matrix PROPERTIES LINKER_LANGUAGE CXX)

target_sources(matrix
  PUBLIC
    prelude.h
    base.hpp
    dense.hpp
    sparse_compressed.hpp
  )
