#pragma once

#include <istream>
#include <sstream>
#include <string>

#include "base.hpp"

template <class DataType, class IndexType>
class DenseMatrix : public Matrix<DataType> {
 private:
  DataType* data;
  IndexType m_num_cols;
  IndexType m_num_rows;

 public:
  DenseMatrix(IndexType num_rows, IndexType num_cols);
  DenseMatrix(IndexType num_rows, IndexType num_cols, std::istream& is);
  ~DenseMatrix();

  inline const DataType& operator()(IndexType i, IndexType j) const;

  void matvec(const DataType* x, DataType* y,
              bool is_transposed = false) const override;

  DenseMatrix operator+(const DenseMatrix& other) const;
};

template <class DataType, class IndexType>
DenseMatrix<DataType, IndexType>::DenseMatrix(IndexType num_rows,
                                              IndexType num_cols)
    : m_num_cols(num_cols), m_num_rows(num_rows) {
  data = new DataType[m_num_rows * m_num_cols];
}

template <class DataType, class IndexType>
DenseMatrix<DataType, IndexType>::DenseMatrix(IndexType num_rows,
                                              IndexType num_cols,
                                              std::istream& is)
    : m_num_cols(num_cols), m_num_rows(num_rows) {
  // TODO: add check
  data = new DataType[m_num_rows * m_num_cols];
  for (IndexType i = 0; i < m_num_rows; i++) {
    std::string row_line;
    std::getline(is, row_line);
    std::istringstream row_stream(std::move(row_line));

    for (IndexType j = 0; j < m_num_cols; j++) {
      row_stream >> data[i * m_num_cols + j];
    }
  }
}

template <class DataType, class IndexType>
DenseMatrix<DataType, IndexType>::~DenseMatrix() {
  delete[] data;
  data = nullptr;
  m_num_cols = m_num_rows = 0;
}

template <class DataType, class IndexType>
inline const DataType& DenseMatrix<DataType, IndexType>::operator()(
    IndexType i, IndexType j) const {
  // TODO: add check
  return data[i * m_num_cols + j];
}

template <class DataType, class IndexType>
void DenseMatrix<DataType, IndexType>::matvec(const DataType* x, DataType* y,
                                              bool is_transposed) const {
  for (IndexType i = 0; i < m_num_rows; i++) {
    y[i] = 0;
    for (IndexType j = 0; j < m_num_cols; j++) {
      if (is_transposed) {
        y[j] += data[i * m_num_cols + j] * x[i];
      } else {
        y[i] += data[i * m_num_cols + j] * x[j];
      }
    }
  }
}

template <class DataType, class IndexType>
DenseMatrix<DataType, IndexType> DenseMatrix<DataType, IndexType>::operator+(
    const DenseMatrix<DataType, IndexType>& other) const {
  DenseMatrix<DataType, IndexType> result(m_num_rows, m_num_cols);
  for (IndexType i = 0; i < m_num_cols * m_num_rows; i++) {
    result.data[i] = data[i] + other.data[i];
  }
  return result;
}
