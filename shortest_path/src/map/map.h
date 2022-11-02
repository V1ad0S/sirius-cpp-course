#pragma once

#include <limits>

struct Point {
  double own_cost;
  double path_cost = std::numeric_limits<double>::max();
  bool visited = false;
  std::size_t prev_i = -1;
  std::size_t prev_j = -1;

  void update(double path_cost, std::size_t prev_i, std::size_t prev_j);
};

typedef Point Type;

class Map {
 private:
  Type* data = nullptr;
  int n = 0;
  int m = 0;

 public:
  ~Map();

  Type& operator()(int i, int j);

  void read_file(const char* filename, int N, int M);
  void print() const;

  friend void find_path(Map& m, int j_start, int j_end);
};
