#include "poly.h"

#include <cmath>
#include <fstream>
#include <sstream>

#include "exceptions.h"

Polygon::Polygon() : points(nullptr), number_of_points(0) {}
Polygon::~Polygon() {
    if (points == nullptr) {
        return;
    }
    delete[] points;
    points = nullptr;
}

int Polygon::check_file(const std::string &filename) const {
    std::ifstream file(filename);
    if (!file) {
        throw ReadingError::CantOpenFile(filename.c_str());
    }

    size_t n = 0;
    double x;
    while (true) {
        file >> x;
        if (file.eof()) {
            break;
        }
        if (file.fail()) {
            throw ReadingError::BadData(int(n / 2));
        }
        n++;
    }

    if (n % 2 != 0) {
        throw ReadingError::NotEnoughData();
    }

    n /= 2;
    if (n < 3) {
        throw ReadingError::NotPolygon(n);
    }
    return n;
}

void Polygon::read_file(const std::string &filename) {
    size_t lines_num = this->check_file(filename);

    std::ifstream file(filename);
    if (!file) {
        throw ReadingError::CantOpenFile(filename.c_str());
    }

    number_of_points = lines_num;
    points = new Point[number_of_points];
    size_t i;
    for (i = 0; i < number_of_points; i++) {
        file >> points[i].x >> points[i].y;
        if (file.fail()) {
            throw ReadingError::BadData(i + 1);
        }
    }

    if (double tmp; (file >> tmp, !file.fail())) {
        throw ReadingError::BadData(i);
    }

    if (!file.eof()) {
        throw ReadingError::BadData(i);
    }
}

bool Polygon::is_point_on_edge(const Point &p, size_t p1_index, size_t p2_index) const {
    Point p1 = points[p1_index], p2 = points[p2_index];
    double ratio_1 = (p.x - p1.x) / (p.y - p1.y);
    double ratio_2 = (p1.x - p2.x) / (p1.y - p2.y);
    if (std::abs(ratio_1 - ratio_2) > 1e-6) {
        return false;
    }

    bool x_between = (std::min(p1.x, p2.x) <= p.x && std::max(p1.x, p2.x) >= p.x);
    bool y_between = (std::min(p1.y, p2.y) <= p.y && std::max(p1.y, p2.y) >= p.y);

    if (x_between && y_between) {
        return true;
    }
    return false;
}

double Polygon::calculate_angle(const Point &p, size_t p1_index, size_t p2_index) const {
    Point v1 = {points[p1_index].x - p.x, points[p1_index].y - p.y};
    Point v2 = {points[p2_index].x - p.x, points[p2_index].y - p.y};
    double cross_product = v1.x * v2.y - v1.y * v2.x;
    double norm_product = (v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y);
    return asin(cross_product / sqrt(norm_product));
}

Polygon::CheckResult Polygon::where_point(const Point &p) const {
    double full_angle = 0.;
    for (size_t i = 1; i < number_of_points; i++) {
        if (is_point_on_edge(p, i - 1, i)) {
            return CheckResult::ON_THE_EDGE;
        }
        full_angle += calculate_angle(p, i - 1, i);
    }
    if (is_point_on_edge(p, number_of_points - 1, 0)) {
        return CheckResult::ON_THE_EDGE;
    }
    full_angle += calculate_angle(p, number_of_points - 1, 0);
    if (full_angle < 3) {
        return CheckResult::OUTSIDE;
    }
    return CheckResult::INSIDE;
}
