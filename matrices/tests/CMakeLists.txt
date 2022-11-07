include_directories(${PROJECT_SOURCE_DIR}/src/matrix)

add_executable(test_dense test_dense.cpp)
add_executable(test_sparse_compressed test_sparse_compressed.cpp)
add_executable(test_sparse_coo test_sparse_coo.cpp)

target_link_libraries(test_dense matrix)
target_link_libraries(test_sparse_compressed matrix)
target_link_libraries(test_sparse_coo matrix)

add_test(
  NAME dense
  COMMAND test_dense "densematrix.txt"
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(
  NAME sparse.compressed
  COMMAND test_sparse_compressed "sparsematrix.txt"
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(
  NAME sparse.coo
  COMMAND test_sparse_coo "coomatrix.txt"
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
