#include <iostream>

#include "map/map.h"

int main() {
  Map M;
  M.read_file("hello", 5, 4);
  M.print();
  find_path(M, 0, 3);
  M.print();
}
