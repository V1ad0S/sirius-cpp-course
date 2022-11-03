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

void Map::from_image(const std::vector<unsigned char>& image, size_t N,
                     size_t M) {
  if (data != nullptr) {
    throw std::runtime_error("already read");
  }
  if (N <= 0 || M <= 0) {
    throw std::runtime_error("wrong size");
  }
  data = new Type[N * M];
  n = N;
  m = M;

  for (size_t i = 2, j = 0, k = 0; i < image.size(); i += 4, k++) {
    if (k == M) {
      k = 0;
      j++;
    }
    data[j * m + k].own_cost = double(image[i]);
  }
}

Type& Map::operator()(int i, int j) {
  if (data == nullptr) {
    throw std::runtime_error("brooo. read data first");
  }
  if (i < 0 || i > n - 1) {
    throw std::runtime_error("`i` index is out of range: 0..n");
  }
  if (j < 0 || j > m - 1) {
    std::cout << "j = \t" << j << std::endl;
    throw std::runtime_error("`j` index is out of range: 0..m");
  }
  return data[i * m + j];
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
}

void draw_map(std::vector<unsigned char>& image, Map& M, const char* outfname) {
  for (size_t i = 0; i < M.n * M.m; i++) {
    image[i * 4 + 2] = (unsigned char)M.data[i].own_cost;
  }
  unsigned error = lodepng::encode(outfname, image, M.n, M.m);
  if (error) {
    std::cout << "encoder error " << error << ": ";
    std::cout << lodepng_error_text(error) << std::endl;
  }
}

void draw_path(std::vector<unsigned char>& image, Map& M, const char* outfname,
               int j_start, int j_end) {
  int back_i = M.n - 1;
  int back_j = j_end;
  while (back_i != 0 || back_j != j_start) {
    int index = (back_i * M.m + back_j) * 4;
    image[index] = (unsigned char)255;
    image[++index] = (unsigned char)0;
    image[++index] = (unsigned char)0;

    Point tmp = M(back_i, back_j);
    back_i = tmp.prev_i;
    back_j = tmp.prev_j;
  }

  unsigned error = lodepng::encode(outfname, image, M.n, M.m);
  if (error) {
    std::cerr << "encoder error " << error << ": ";
    std::cerr << lodepng_error_text(error) << std::endl;
  }
}

void Map::print_map() const {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      std::cout << data[i * m + j].own_cost << ' ';
    }
    std::cout << std::endl;
  }
}
