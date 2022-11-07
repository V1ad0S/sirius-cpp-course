#pragma once

// enum class Multiplication {
//   NonTransposed,
//   Transposed,
// };

template <class DataType, class IndexType>
class Matrix {
 public:
  virtual ~Matrix(){};

  virtual IndexType get_num_rows() const = 0;
  virtual IndexType get_num_cols() const = 0;
  // is not possible to use templates in virtual function
  virtual void matvec(const DataType* x, DataType* y,
                      bool is_transposed = false) const = 0;
};
