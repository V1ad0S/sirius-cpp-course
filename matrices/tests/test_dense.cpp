#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "dense.hpp"

int main(int, char *argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  std::getline(ifs, line);

  int rows, cols;
  std::istringstream iss(line);
  iss >> rows >> cols;
  std::getline(ifs, line);

  DenseMatrix<int, int> D(rows, cols, ifs);
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
  {
    std::cout << std::endl;
    DenseMatrix<int, int> C = D + D;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        std::cout << C(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }

  return 0;
}
