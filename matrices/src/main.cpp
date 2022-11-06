#include <fstream>
#include <iostream>
#include <vector>

#include "matrix/dense.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) return -1;

  std::ifstream ifs(argv[1]);
  std::string line;
  std::getline(ifs, line);

  int rows, cols;
  std::istringstream iss(line);
  iss >> rows >> cols;

  DenseMatrix<int, int> D = DenseMatrix<int, int>(rows, cols, ifs);
  {
    std::vector<int> x(cols, 1);
    std::vector<int> y(rows, 0);

    D.matvec(x.data(), y.data());

    for (auto elem : y) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }
  {
    std::vector<int> x(rows, 1);
    std::vector<int> y(cols, 0);

    D.matvec(x.data(), y.data(), true);

    for (auto elem : y) {
      std::cout << elem << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
