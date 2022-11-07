#pragma once

#include <algorithm>
#include <istream>
#include <sstream>
#include <vector>

#include "base.hpp"

template <class DataType, class IndexType, class OffsetType>
class CompressedSparse : public Matrix<DataType, IndexType> {
  struct Element {
    IndexType index;
    DataType value;

    inline bool operator<(const Element& other) const {
      return index < other.index;
    }
    inline bool operator==(const Element& other) const {
      return index == other.index;
    }
  };

 public:
  CompressedSparse(IndexType offsets_num, std::istream& is);
  inline IndexType get_num_rows() const = 0;
  inline IndexType get_num_cols() const = 0;

 protected:
  CompressedSparse() {}

  std::vector<OffsetType> m_offsets;
  std::vector<Element> m_elements;
  static std::vector<OffsetType> read_offsets(IndexType m, std::istream& is);
  static std::vector<Element> read_elements(OffsetType nz, std::istream& is);
  IndexType calc_max_index() const;
  void sort();
  void cs_matvec(const DataType* x, DataType* y, bool direct) const;
  void cs_add(const CompressedSparse& other, CompressedSparse& result) const;
};

template <class DataType, class IndexType, class OffsetType>
class CSRMatrix : public CompressedSparse<DataType, IndexType, OffsetType> {
 private:
  IndexType m_num_cols;

  CSRMatrix() = default;

 public:
  CSRMatrix(IndexType num_rows, IndexType num_cols, std::istream& is);

  CSRMatrix operator+(const CSRMatrix& other) const;

  inline IndexType get_num_rows() const override {
    return this->m_offsets.size() - 1;
  }
  inline IndexType get_num_cols() const override { return m_num_cols; }
  void matvec(const DataType* x, DataType* y,
              bool is_transposed = false) const override;
};

template <class DataType, class IndexType, class OffsetType>
class CSCMatrix : public CompressedSparse<DataType, IndexType, OffsetType> {
 private:
  IndexType m_num_rows;

  CSCMatrix() = default;

 public:
  CSCMatrix(IndexType num_rows, IndexType num_cols, std::istream& is);

  CSCMatrix operator+(const CSCMatrix& other) const;

  inline IndexType get_num_rows() const override { return m_num_rows; }
  inline IndexType get_num_cols() const override {
    return this->m_offsets.size() - 1;
  }
  void matvec(const DataType* x, DataType* y,
              bool is_transposed = false) const override;
};

template <class DataType, class IndexType, class OffsetType>
CompressedSparse<DataType, IndexType, OffsetType>::CompressedSparse(
    IndexType offsets_num, std::istream& is)

    : m_offsets(read_offsets(offsets_num, is)),
      m_elements(read_elements(m_offsets[offsets_num], is)) {
  sort();
}

template <class DataType, class IndexType, class OffsetType>
std::vector<OffsetType>
CompressedSparse<DataType, IndexType, OffsetType>::read_offsets(
    IndexType m, std::istream& is) {
  std::vector<OffsetType> offsets(m + 1);

  std::string line;
  std::getline(is, line);
  std::istringstream line_stream(std::move(line));

  for (auto& offset : offsets) {
    line_stream >> offset;
    if (line_stream.fail()) {
      throw std::runtime_error("Can not read enough offsets");
    }
  }
  if (OffsetType tmp; (line_stream >> tmp, !line_stream.fail())) {
    throw std::runtime_error("Too many offsets");
  }
  if (!line_stream.eof()) {
    throw std::runtime_error("Offsets string has some other info");
  }
  if (offsets[0] != 0) {
    throw std::runtime_error("Fisrst offset is not zero");
  }
  if (!std::is_sorted(offsets.begin(), offsets.end())) {
    throw std::runtime_error("Offsets are not sorted");
  }

  return offsets;
}

template <class DataType, class IndexType, class OffsetType>
std::vector<typename CompressedSparse<DataType, IndexType, OffsetType>::Element>
CompressedSparse<DataType, IndexType, OffsetType>::read_elements(
    OffsetType nz, std::istream& is) {
  std::vector<Element> elements(nz);
  {
    std::string line;
    std::getline(is, line);
    std::istringstream line_stream(std::move(line));
    for (auto& element : elements) {
      line_stream >> element.index;
      if (line_stream.fail()) {
        throw std::runtime_error("Cannot read enough indexes");
      }
      if (element.index < 0) {
        throw std::runtime_error("Negative column number");
      }
    }
    if (IndexType tmp; (line_stream >> tmp, !line_stream.fail())) {
      throw std::runtime_error("Too many indexes");
    }
    if (!(line_stream.eof())) {
      throw std::runtime_error("Indexes string has some other info");
    }
  }

  {
    std::string line;
    std::getline(is, line);
    std::istringstream line_stream(std::move(line));

    for (auto& element : elements) {
      line_stream >> element.value;
      if (line_stream.fail()) {
        throw std::runtime_error("Can not read enough values");
      }
    }
    if (DataType tmp; (line_stream >> tmp, !line_stream.fail())) {
      throw std::runtime_error("Too many values");
    }
    if (!line_stream.eof()) {
      throw std::runtime_error("Values string has some other info");
    }
  }

  return elements;
}

template <class DataType, class IndexType, class OffsetType>
IndexType CompressedSparse<DataType, IndexType, OffsetType>::calc_max_index()
    const {
  IndexType max_index = 0;
  for (auto& elem : m_elements) {
    max_index = std::max(max_index, elem.index);
  }
  return max_index;
}

template <class DataType, class IndexType, class OffsetType>
void CompressedSparse<DataType, IndexType, OffsetType>::sort() {
  for (size_t i = 0; i < m_offsets.size() - 1; i++) {
    auto start = m_elements.begin() + m_offsets[i];
    auto end = m_elements.begin() + m_offsets[i + 1];
    if (!std::is_sorted(start, end)) {
      std::sort(start, end);
    }
    if (std::unique(start, end) != end) {
      throw std::runtime_error(
          "Two or more elements with same index in the same vector");
    }
  }
}

template <class DataType, class IndexType, class OffsetType>
void CompressedSparse<DataType, IndexType, OffsetType>::cs_matvec(
    const DataType* x, DataType* y, bool direct) const {
  for (IndexType i = 0; i < get_num_rows(); i++) {
    auto start = m_elements.begin() + m_offsets[i];
    auto end = m_elements.begin() + m_offsets[i + 1];
    for (; start < end; ++start) {
      const IndexType& j = start->index;
      const DataType& a = start->value;
      if (direct) {
        y[i] += a * x[j];
      } else {
        y[j] += a * x[i];
      }
    }
  }
}

template <class DataType, class IndexType, class OffsetType>
void CompressedSparse<DataType, IndexType, OffsetType>::cs_add(
    const CompressedSparse& other, CompressedSparse& result) const {
  if (get_num_cols() != other.get_num_cols() ||
      get_num_rows() != other.get_num_rows()) {
    throw std::runtime_error("Can not sum matrices of different sizes");
  }
  result.m_offsets.resize(other.m_offsets.size(), 0);

  for (size_t i = 0; i < m_offsets.size() - 1; i++) {
    auto start_this = m_elements.begin() + m_offsets[i];
    auto end_this = m_elements.begin() + m_offsets[i + 1];

    auto start_other = other.m_elements.begin() + other.m_offsets[i];
    auto end_other = other.m_elements.begin() + other.m_offsets[i + 1];

    while (start_this < end_this && start_other < end_other) {
      while (start_this < end_this && *start_this < *start_other) {
        result.m_elements.push_back(*start_this);
        result.m_offsets[i + 1]++;
        ++start_this;
      }
      while (start_this < end_this && start_other < end_other &&
             *start_this == *start_other) {
        Element t = *start_this;
        t.value += start_other->value;
        result.m_elements.push_back(t);
        result.m_offsets[i + 1]++;
        ++start_this;
        ++start_other;
      }
      if (start_this == end_this) break;
      while (start_other < end_other && *start_other < *start_this) {
        result.m_elements.push_back(*start_other);
        result.m_offsets[i + 1]++;
        ++start_other;
      }
    }

    while (start_this < end_this) {
      result.m_elements.push_back(*start_this);
      result.m_offsets[i + 1]++;
      ++start_this;
    }
    while (start_other < end_other) {
      result.m_elements.push_back(*start_other);
      result.m_offsets[i + 1]++;
      ++start_other;
    }

    result.m_offsets[i + 1] += result.m_offsets[i];
  }
}

template <class DataType, class IndexType, class OffsetType>
CSCMatrix<DataType, IndexType, OffsetType>::CSCMatrix(IndexType num_rows,
                                                      IndexType num_cols,
                                                      std::istream& is)
    : CompressedSparse<DataType, IndexType, OffsetType>(num_cols, is) {
  if (this->calc_max_index() >= num_rows) {
    throw std::runtime_error(
        "Matrix has rows with indexes larger than number of rows");
  }
  m_num_rows = num_rows;
}

template <class DataType, class IndexType, class OffsetType>
void CSCMatrix<DataType, IndexType, OffsetType>::matvec(
    const DataType* x, DataType* y, bool is_transposed) const {
  this->cs_matvec(x, y, is_transposed);
}

template <class DataType, class IndexType, class OffsetType>
CSCMatrix<DataType, IndexType, OffsetType>
CSCMatrix<DataType, IndexType, OffsetType>::operator+(
    const CSCMatrix& other) const {
  CSCMatrix<DataType, IndexType, OffsetType> result;
  result.m_num_rows = m_num_rows;
  this->cs_add(other, result);
  return result;
}

template <class DataType, class IndexType, class OffsetType>
CSRMatrix<DataType, IndexType, OffsetType>::CSRMatrix(IndexType num_rows,
                                                      IndexType num_cols,
                                                      std::istream& is)
    : CompressedSparse<DataType, IndexType, OffsetType>(num_rows, is) {
  if (this->calc_max_index() >= num_cols) {
    throw std::runtime_error(
        "Matrix has columns with indexes larger than number of columns");
  }
  m_num_cols = num_cols;
}

template <class DataType, class IndexType, class OffsetType>
void CSRMatrix<DataType, IndexType, OffsetType>::matvec(
    const DataType* x, DataType* y, bool is_transposed) const {
  this->cs_matvec(x, y, !is_transposed);
}

template <class DataType, class IndexType, class OffsetType>
CSRMatrix<DataType, IndexType, OffsetType>
CSRMatrix<DataType, IndexType, OffsetType>::operator+(
    const CSRMatrix& other) const {
  CSRMatrix<DataType, IndexType, OffsetType> result;
  result.m_num_cols = m_num_cols;
  this->cs_add(other, result);
  return result;
}
