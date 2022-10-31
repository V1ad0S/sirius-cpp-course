#include "exceptions.h"

#include <sstream>

namespace ReadingError {

const char* BaseException::what() const noexcept { return message; }

CantOpenFile::CantOpenFile(const char* fname) {
  std::ostringstream oss;
  oss << "Can't open the file: " << fname;
  message = oss.str().c_str();
}

BadData::BadData(int line_num) {
  std::ostringstream oss;
  oss << "Bad data on the line #" << line_num;
  message = oss.str().c_str();
}

}  // namespace ReadingError
