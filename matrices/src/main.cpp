#include <fstream>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <vector>

#include "matrix/base.hpp"
#include "matrix/prelude.h"

using Dense = DenseMatrix<int, int>;
using CSC = CSCMatrix<int, int, int>;
using CSR = CSRMatrix<int, int, int>;
using COO = COOMatrix<int, int>;

void vec_sum(const std::vector<int> &x, std::vector<int> &y) {
  auto px = x.begin();
  for (auto py = y.begin(); py < y.end(); px++, py++) {
    *py += *px;
  }
}

std::vector<int> multiplicate(const Matrix<int, int> &M) {
  std::vector<int> x(M.get_num_cols(), 1);
  std::vector<int> y1(M.get_num_cols(), 0);
  std::vector<int> y2(M.get_num_cols(), 0);

  M.matvec(x.data(), y1.data());
  M.matvec(x.data(), y2.data(), true);
  vec_sum(y1, y2);
  return y2;
}

int main(int argc, char *argv[]) {
  if (argc < 3) return -1;

  Matrix<int, int> *M[2];

  std::string prev;
  std::string line;

  for (int i = 1; i < argc; i++) {
    std::ifstream ifs(argv[i]);
    std::getline(ifs, line);

    int rows, cols;
    std::istringstream iss(line);
    iss >> rows >> cols;

    if (rows != cols) {
      throw std::runtime_error("Here should be square matrices");
    }

    std::getline(ifs, line);
    std::cout << rows << ' ' << cols << ' ' << line << std::endl;

    if (line == "dense") {
      M[i - 1] = new Dense(rows, cols, ifs);
    } else if (line == "csc") {
      M[i - 1] = new CSC(rows, cols, ifs);
    } else if (line == "csr") {
      M[i - 1] = new CSR(rows, cols, ifs);
    } else if (line == "coo") {
      M[i - 1] = new COO(rows, cols, ifs);
    }
  }

  std::vector<int> res;

  if (line == prev) {
    if (line == "dense") {
      Dense C = (*dynamic_cast<Dense *>(M[0])) + (*dynamic_cast<Dense *>(M[1]));
      res = multiplicate(C);
    } else if (line == "csc") {
      CSC C = (*dynamic_cast<CSC *>(M[0])) + (*dynamic_cast<CSC *>(M[1]));
      res = multiplicate(C);
    } else if (line == "csr") {
      CSR C = (*dynamic_cast<CSR *>(M[0])) + (*dynamic_cast<CSR *>(M[1]));
      res = multiplicate(C);
    } else if (line == "coo") {
      COO C = (*dynamic_cast<COO *>(M[0])) + (*dynamic_cast<COO *>(M[1]));
      res = multiplicate(C);
    }
  } else {
    res = multiplicate(*M[0]);
    std::vector<int> tmp = multiplicate(*M[1]);
    vec_sum(tmp, res);
  }

  for (auto el : res) {
    std::cout << el << ' ';
  }
  std::cout << std::endl;

  delete M[0];
  delete M[1];

  return 0;
}
