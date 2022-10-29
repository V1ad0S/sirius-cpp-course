#include "exceptions.h"

#include <sstream>

namespace ReadingError {

CantOpenFile::CantOpenFile(const char *fname) : filename(fname) {
    std::ostringstream oss;
    oss << "Can't open the file: " << filename;
    this->message = oss.str();
}
const char *CantOpenFile::what() const noexcept { return message.c_str(); }

BadData::BadData(const int ln) : line_number(ln) {
    std::ostringstream oss;
    oss << "Wrong data detected: line #" << line_number;
    this->message = oss.str();
}

const char *BadData::what() const noexcept { return message.c_str(); }

const char *NotEnoughData::what() const noexcept { return "Not enough data in file"; }

NotPolygon::NotPolygon(const size_t n) : number_of_points(n) {
    std::ostringstream oss;
    oss << "Less than 3 points given: n = " << number_of_points;
    this->message = oss.str();
}

const char *NotPolygon::what() const noexcept { return message.c_str(); }

}  // namespace ReadingError
