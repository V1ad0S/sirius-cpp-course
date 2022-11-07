#pragma once

#include <algorithm>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "base.hpp"

template <class DataType, class IndexType>
class COOMatrix : public Matrix<DataType, IndexType> {
  struct Element {
    IndexType row_index;
    IndexType col_index;
    DataType value;

    inline bool operator<(const Element& other) const {
      if (row_index < other.row_index) {
        return true;
      }
      return col_index < other.col_index;
    }
    inline bool operator==(const Element& other) const {
      return (row_index == other.row_index) && (col_index == other.col_index);
    }
  };

 private:
  COOMatrix() = default;

  IndexType m_num_rows;
  IndexType m_num_cols;
  std::vector<Element> m_elements;

 public:
  COOMatrix(IndexType num_rows, IndexType num_cols, std::istream& is);

  void sort();

  inline IndexType get_num_rows() const override { return m_num_rows; }
  inline IndexType get_num_cols() const override { return m_num_cols; }
  void matvec(const DataType* x, DataType* y,
              bool is_transposed = false) const override;
  COOMatrix operator+(const COOMatrix& other) const;
};

template <class DataType, class IndexType>
COOMatrix<DataType, IndexType>::COOMatrix(IndexType num_rows,
                                          IndexType num_cols,
                                          std::istream& is) {
  // TODO: add check
  // TODO: read number of elements first then reread
  std::string line;
  std::getline(is, line);
  while (!is.fail()) {
    std::istringstream line_stream(std::move(line));

    Element el;
    line_stream >> el.row_index >> el.col_index >> el.value;
    m_elements.push_back(el);
    std::getline(is, line);
  }
  if (!is.eof()) {
    throw std::runtime_error("cant read enough elements");
  }

  sort();

  Element last = m_elements.back();
  if (num_rows < last.row_index) {
    throw std::runtime_error(
        "File contains row index greater than max row index");
  }
  if (num_cols < last.col_index) {
    throw std::runtime_error(
        "File contains column index greater than max column index");
  }
  m_num_rows = num_rows;
  m_num_cols = num_cols;
}

template <class DataType, class IndexType>
void COOMatrix<DataType, IndexType>::sort() {
  auto start = m_elements.begin();
  auto end = m_elements.end();
  if (!std::is_sorted(start, end)) {
    std::sort(start, end);
  }

  if (std::unique(start, end) != end) {
    throw std::runtime_error("Two or more elements with same indexes");
  }
}

template <class DataType, class IndexType>
void COOMatrix<DataType, IndexType>::matvec(const DataType* x, DataType* y,
                                            bool is_transposed) const {
  for (auto& el : m_elements) {
    if (is_transposed) {
      y[el.col_index] += el.value * x[el.row_index];
    } else {
      y[el.row_index] += el.value * x[el.col_index];
    }
  }
}

template <class DataType, class IndexType>
COOMatrix<DataType, IndexType> COOMatrix<DataType, IndexType>::operator+(
    const COOMatrix& other) const {
  // if (col_number() != other.col_number() ||
  //     row_number() != other.row_number()) {
  //   throw std::runtime_error("Can not sum matrices of different sizes");
  // }
  COOMatrix result;
  result.m_num_rows = m_num_rows;
  result.m_num_cols = m_num_cols;

  auto start_this = m_elements.begin();
  auto end_this = m_elements.end();
  auto start_other = other.m_elements.begin();
  auto end_other = other.m_elements.end();

  while (start_this < end_this && start_other < end_other) {
    while (start_this < end_this && *start_this < *start_other) {
      result.m_elements.push_back(*start_this);
      ++start_this;
    }
    while (start_this < end_this && start_other < end_other &&
           *start_this == *start_other) {
      Element tmp = *start_this;
      tmp.value += start_other->value;
      result.m_elements.push_back(tmp);
      ++start_this;
      ++start_other;
    }
    if (start_this == end_this) break;
    while (start_other < end_other && *start_other < *start_this) {
      result.m_elements.push_back(*start_other);
      ++start_other;
    }
  }

  while (start_this < end_this) {
    result.m_elements.push_back(*start_this);
    ++start_this;
  }
  while (start_other < end_other) {
    result.m_elements.push_back(*start_other);
    ++start_other;
  }

  return result;
}
