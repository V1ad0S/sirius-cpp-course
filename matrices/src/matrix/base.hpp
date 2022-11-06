#pragma once

// enum class Multiplication {
//   NonTransposed,
//   Transposed,
// };

template <class DataType>
class Matrix {
 public:
  virtual ~Matrix(){};

  // is not possible to use templates in virtual function
  virtual void matvec(const DataType* x, DataType* y,
                      bool is_transposed = false) const = 0;
};
