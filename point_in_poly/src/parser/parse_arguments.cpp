#include "parse_arguments.h"

#include <sstream>

namespace ParseError {

const char* BaseException::get_usage_help() const {
    return "Usage: ./main <filename> <x_coord> <y_coord>";
}

NotEnoughArguments::NotEnoughArguments(int _given) : given_argc(_given) {
    std::ostringstream oss;
    oss << "Not enough arguments. ";
    oss << "Given: " << given_argc - 1;
    oss << '\n' << get_usage_help();
    message = oss.str();
}

const char* NotEnoughArguments::what() const noexcept { return message.c_str(); }

TooManyArguments::TooManyArguments(int _given) : given_argc(_given) {
    std::ostringstream oss;
    oss << "Too many arguments. ";
    oss << "Given: " << given_argc - 1;
    oss << '\n' << get_usage_help();
    this->message = oss.str();
}

const char* TooManyArguments::what() const noexcept { return message.c_str(); }

NotANumber::NotANumber(char _sym, const char* _given) : symbol(_sym), given_value(_given) {
    std::ostringstream oss;
    oss << "Given argument can't be coverted to double: ";
    oss << symbol << " = " << given_value;
    oss << '\n' << get_usage_help();
    message = oss.str();
}

const char* NotANumber::what() const noexcept { return message.c_str(); }

}  // namespace ParseError

void parse_arguments(int argc, char** argv, std::string& filename, double& x, double& y) {
    const int needed_argc = 4;
    if (argc < needed_argc) {
        throw ParseError::NotEnoughArguments(argc);
    }
    if (argc > needed_argc) {
        throw ParseError::TooManyArguments(argc);
    }
    filename = std::string(argv[1]);
    {
        std::istringstream iss(argv[2]);
        iss >> x;
        if (iss.fail()) {
            throw ParseError::NotANumber('x', argv[2]);
        }
    }
    {
        std::istringstream iss(argv[3]);
        iss >> y;
        if (iss.fail()) {
            throw ParseError::NotANumber('y', argv[3]);
        }
    }
}
