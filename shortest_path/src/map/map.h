#pragma once

#include <limits>
#include <vector>

#include "lodepng.h"

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

  void from_image(const std::vector<unsigned char>& image, size_t N, size_t M);
  void print() const;

  friend void find_path(Map& M, int j_start, int j_end);
  friend void draw_map(std::vector<unsigned char>& image, Map& M,
                       const char* outfname);
  friend void draw_path(std::vector<unsigned char>& image, Map& M,
                        const char* outfname, int j_start, int j_end);
  void print_map() const;
};
