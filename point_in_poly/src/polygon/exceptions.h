#pragma once

#include <exception>
#include <string>

namespace ReadingError {

class BaseException : public std::exception {
   public:
    virtual const char* what() const noexcept = 0;
};

class CantOpenFile : public BaseException {
   private:
    std::string message;
    const char* filename;

   public:
    CantOpenFile(const char* filename);
    const char* what() const noexcept;
};

class BadData : public BaseException {
   private:
    std::string message;
    const int line_number;

   public:
    BadData(const int line_number);
    const char* what() const noexcept;
};

class NotEnoughData : public BaseException {
   public:
    const char* what() const noexcept;
};

class NotPolygon : public BaseException {
   private:
    std::string message;
    const size_t number_of_points;

   public:
    NotPolygon(const size_t n);
    const char* what() const noexcept;
};

}  // namespace ReadingError
