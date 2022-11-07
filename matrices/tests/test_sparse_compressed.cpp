#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "sparse_compressed.hpp"

enum class CSType {
  CSC,
  CSR,
};

int test(char *fname, CSType type) {
  std::ifstream ifs(fname);
  std::string line;
  std::getline(ifs, line);

  int rows, cols;
  std::istringstream iss(line);
  iss >> rows >> cols;
  std::getline(ifs, line);
  std::cout << rows << ' ' << cols << ' ' << std::endl;

  CompressedSparse<int, int, int> *sparse_matrix;
  CSCMatrix<int, int, int> *csc;
  CSRMatrix<int, int, int> *csr;
  switch (type) {
    case CSType::CSC:
      std::cout << "CSC matrix:" << std::endl;
      csc = new CSCMatrix<int, int, int>(rows, cols, ifs);
      sparse_matrix = csc;
      {
        std::vector<int> x(sparse_matrix->get_num_rows(), 1);
        std::vector<int> y(sparse_matrix->get_num_cols(), 0);

        std::cout << "Doubled" << std::endl;
        (*csc + *csc).matvec(x.data(), y.data());
        for (auto v : y) {
          std::cout << v << ' ';
        }
        std::cout << std::endl;
      }
      break;
    case CSType::CSR:
      csr = new CSRMatrix<int, int, int>(rows, cols, ifs);
      sparse_matrix = csr;
      {
        std::vector<int> x(sparse_matrix->get_num_rows(), 1);
        std::vector<int> y(sparse_matrix->get_num_cols(), 0);

        std::cout << "Doubled" << std::endl;
        (*csr + *csr).matvec(x.data(), y.data());
        for (auto v : y) {
          std::cout << v << ' ';
        }
        std::cout << std::endl;
      }
      break;
    default:
      return -1;
  }

  {
    std::vector<int> x(sparse_matrix->get_num_cols(), 1);
    std::vector<int> y(sparse_matrix->get_num_rows(), 0);

    std::cout << "NonTransposed:" << std::endl;
    sparse_matrix->matvec(x.data(), y.data());
    for (auto v : y) {
      std::cout << v << ' ';
    }
    std::cout << std::endl;
  }
  {
    std::vector<int> x(sparse_matrix->get_num_rows(), 1);
    std::vector<int> y(sparse_matrix->get_num_cols(), 0);

    std::cout << "Transposed:" << std::endl;
    sparse_matrix->matvec(x.data(), y.data(), true);
    for (auto v : y) {
      std::cout << v << ' ';
    }
    std::cout << std::endl;
  }
  return 0;
}

int main(int, char *argv[]) {
  test(argv[1], CSType::CSC);
  std::cout << std::endl;
  test(argv[1], CSType::CSR);
  return 0;
}
