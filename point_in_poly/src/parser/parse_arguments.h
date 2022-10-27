#pragma once

#include <exception>
#include <string>

namespace ParseError {

class BaseException : public std::exception {
   public:
    const char* get_usage_help() const;
    virtual const char* what() const noexcept = 0;
};

class NotEnoughArguments : public BaseException {
   private:
    const int given_argc;
    std::string message;

   public:
    NotEnoughArguments(const int given_argc);
    virtual const char* what() const noexcept;
};

class TooManyArguments : public BaseException {
   private:
    const int given_argc;
    std::string message;

   public:
    TooManyArguments(const int given_argc);
    virtual const char* what() const noexcept;
};

class NotANumber : public BaseException {
   private:
    const char symbol;
    const char* given_value;
    std::string message;

   public:
    NotANumber(const char symbol, const char* given_value);
    virtual const char* what() const noexcept;
};

}  // namespace ParseError

void parse_arguments(int argc, char** argv, std::string& filename, double& x, double& y);
