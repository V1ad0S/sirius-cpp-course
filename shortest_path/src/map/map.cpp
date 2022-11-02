#include "map.h"

#include <fstream>
#include <iostream>

Map::~Map() {
  if (data == nullptr) {
    return;
  }
  delete[] data;
  data = nullptr;
}

void Map::read_file(const char* filename, int N, int M) {
  if (N <= 0 || M <= 0) {
    throw "wrong size";
  }
  data = new Type[N * M];
  n = N;
  m = M;

  // TODO: read_file
  std::cerr << "File not read" << std::endl;
  for (int i = 0; i < n * m; i++) {
    data[i] = i;
  }
}

const Type& Map::operator()(int i, int j) const {
  if (data == nullptr) {
    throw "brooo. read data first";
  }
  if (i < 0 || i > n - 1) {
    throw "`i` index is out of range: 0..n";
  }
  if (j < 0 || j > m - 1) {
    throw "`j` index is out of range: 0..m";
  }
  return data[i * n + j];
}
