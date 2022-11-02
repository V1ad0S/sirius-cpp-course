#include <iostream>

#include "map/map.h"

int main() {
  Map M;
  M.read_file("hello", 5, 4);
  std::cout << M(1, 2) << std::endl;
  std::cout << M(3, 2) << std::endl;
  std::cout << M(2, 3) << std::endl;
}
