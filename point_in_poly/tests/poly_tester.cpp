#include <exception>
#include <fstream>
#include <iostream>

#include "poly.h"

const std::string CASES_DIR = "poly_test_cases/";

int test_points(Polygon::CheckResult actual_result) {
    std::string test_name;
    switch (actual_result) {
        case Polygon::CheckResult::OUTSIDE:
            test_name = "outside_points";
            break;
        case Polygon::CheckResult::INSIDE:
            test_name = "inside_points";
            break;
        case Polygon::CheckResult::ON_THE_EDGE:
            test_name = "edge_points";
            break;
        default:
            std::cerr << "Wrong test case" << std::endl;
            return -3;
    }

    std::ifstream ifs(CASES_DIR + test_name + ".txt");
    Polygon poly;
    poly.read_file("poly_test_cases/data.txt");
    double x, y;
    int point_number = 1;
    while (!ifs.eof()) {
        ifs >> x >> y;
        Point p = {x, y};
        if (auto result = poly.where_point(p); result != actual_result) {
            std::cout << "Wrong result on point #" << point_number << ": ";
            std::cout << '(' << x << ", " << y << ')';
            std::cout << "\tExpected: " << int(actual_result) << "\t Got: " << int(result);
            std::cout << std::endl;
            return -1;
        }
        point_number++;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Not enough arguments!" << std::endl;
        return -3;
    }
    auto test_case = static_cast<Polygon::CheckResult>(atoi(argv[1]));

    int result = test_points(test_case);
    if (result) {
        return -1;
    }
    return 0;
}
