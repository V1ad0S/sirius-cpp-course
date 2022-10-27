#include <iostream>

#include "parser/parse_arguments.h"
#include "polygon/exceptions.h"
#include "polygon/poly.h"

// TODO: Bad Alloc check

int main(int argc, char* argv[]) {
    std::string filename;
    double x, y;
    try {
        parse_arguments(argc, argv, filename, x, y);
    } catch (ParseError::BaseException& ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    Point p = {x, y};
    Polygon poly;
    try {
        poly.read_file(filename);
    } catch (ReadingError::BaseException& ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    }

    switch (poly.where_point(p)) {
        case Polygon::CheckResult::INSIDE:
            std::cout << "Inside" << std::endl;
            break;
        case Polygon::CheckResult::OUTSIDE:
            std::cout << "Outside" << std::endl;
            break;
        case Polygon::CheckResult::ON_THE_EDGE:
            std::cout << "On the edge" << std::endl;
            break;
    }

    return 0;
}
