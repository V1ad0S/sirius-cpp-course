#include "map.h"

#include <fstream>
#include <iostream>

void Point::update(double path_cost, std::size_t prev_i, std::size_t prev_j) {
  this->path_cost = path_cost;
  this->prev_i = prev_i;
  this->prev_j = prev_j;
}

Map::~Map() {
  if (data == nullptr) {
    return;
  }
  delete[] data;
  data = nullptr;
}

void Map::read_file(const char* filename, int N, int M) {
  if (data != nullptr) {
    throw "already read";
  }
  if (N <= 0 || M <= 0) {
    throw "wrong size";
  }
  data = new Type[N * M];
  n = N;
  m = M;

  // TODO: read_file
  std::cerr << "File not read" << std::endl;
  for (int i = 0; i < n * m; i++) {
    data[i].own_cost = i;
  }
}

Type& Map::operator()(int i, int j) {
  if (data == nullptr) {
    throw "brooo. read data first";
  }
  if (i < 0 || i > n - 1) {
    throw "`i` index is out of range: 0..n";
  }
  if (j < 0 || j > m - 1) {
    throw "`j` index is out of range: 0..m";
  }
  return data[i * m + j];
}

void Map::print() const {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << data[i * m + j].own_cost << "\t";
    }
    std::cout << "\n";
  }
}

void find_path(Map& M, int j_start, int j_end) {
  int visited_number = 0;
  M(0, j_start).path_cost = 0;

  while (visited_number < M.m * M.n) {
    int I, J;
    double min_path_cost = std::numeric_limits<double>::max();
    for (int i = 0; i < M.n; i++) {
      for (int j = 0; j < M.m; j++) {
        if (!M(i, j).visited) {
          if (M(i, j).path_cost < min_path_cost) {
            I = i;
            J = j;
            min_path_cost = M(i, j).path_cost;
          }
        }
      }
    }
    // todo: check if I, J not assigned
    double pvc = M(I, J).path_cost + M(I, J).own_cost;

    if (I < M.n - 1) {
      if (M(I + 1, J).path_cost > pvc) {
        if (M(I + 1, J).visited) {
          visited_number++;
        }
        M(I + 1, J).update(pvc, I, J);
      }
    }

    if (J > 0) {
      if (M(I, J - 1).path_cost > pvc) {
        if (M(I, J - 1).visited) {
          visited_number++;
        }
        M(I, J - 1).update(pvc, I, J);
      }
    }

    if (J < M.m - 1) {
      if (M(I, J + 1).path_cost > pvc) {
        if (M(I, J + 1).visited) {
          visited_number++;
        }
        M(I, J + 1).update(pvc, I, J);
      }
    }
    M(I, J).visited = true;
    visited_number++;
  }

  int back_i = M.n - 1;
  int back_j = j_end;
  while (back_i != 0 || back_j != j_start) {
    std::cout << '(' << back_i << ", " << back_j << ')' << std::endl;
    Point tmp = M(back_i, back_j);
    back_i = tmp.prev_i;
    back_j = tmp.prev_j;
  }
}
