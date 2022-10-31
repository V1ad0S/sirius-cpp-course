#include <exception>

namespace ReadingError {

class BaseException : public std::exception {
 protected:
  const char* message;

 public:
  const char* what() const noexcept;
};

class CantOpenFile : public BaseException {
 public:
  CantOpenFile(const char* fname);
};

class BadData : public BaseException {
 public:
  BadData(int line_number);
};

}  // namespace ReadingError
