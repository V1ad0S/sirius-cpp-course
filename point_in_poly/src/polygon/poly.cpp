#include "poly.h"

#include <cmath>
#include <fstream>
#include <sstream>

#include "exceptions.h"

static inline double dot_product(const Point &p1, const Point &p2) {
    return p1.x * p2.x + p1.y * p2.y;
}
static inline bool points_is_equal(const Point &p1, const Point &p2) {
    Point diff = {p1.x - p2.x, p1.y - p2.y};
    double diff_norm, p1_norm, p2_norm;
    diff_norm = std::sqrt(dot_product(diff, diff));
    p1_norm = std::sqrt(dot_product(p1, p1));
    p2_norm = std::sqrt(dot_product(p2, p2));

    return diff_norm < 100 * std::numeric_limits<double>::epsilon() * std::max(p1_norm, p2_norm);
}
static inline bool angles_is_equal(double theta_1, double theta_2) {
    return (std::abs(theta_1 - theta_2) / M_PI) < 100 * std::numeric_limits<double>::epsilon();
}

Point Point::operator-(const Point &other) { return {this->x - other.x, this->y - other.y}; }

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

double Polygon::calculate_angle(const Point &p, size_t p1_index, size_t p2_index) const {
    Point p1 = points[p1_index], p2 = points[p2_index];

    if (points_is_equal(p1, p2)) {
        throw ReadingError::BadData(p2_index);
    }
    if (points_is_equal(p, p1) || points_is_equal(p, p2)) {
        return M_PI;
    }

    Point v1 = p1 - p;
    Point v2 = p2 - p;

    double ratio = dot_product(v1, v2) / dot_product(v2, v2);
    Point proj = {ratio * v2.x, ratio * v2.y};
    Point ort = v1 - proj;

    double adjacent = std::sqrt(proj.x * proj.x + proj.y * proj.y);
    double opposite = std::sqrt(ort.x * ort.x + ort.y * ort.y);
    double angle = atan2(opposite, adjacent);

    if (ratio < 0) {
        angle = M_PI - angle;
    }

    double cross_product = v1.x * v2.y - v1.y * v2.x;
    if (cross_product > 0) {
        angle *= -1;
    }

    return angle;
}

Polygon::CheckResult Polygon::where_point(const Point &p) const {
    double angle, full_angle = 0.;
    for (size_t i = 1; i < number_of_points; i++) {
        angle = calculate_angle(p, i - 1, i);
        if (angles_is_equal(std::abs(angle), M_PI)) {
            return CheckResult::ON_THE_EDGE;
        }
        full_angle += angle;
    }
    angle = calculate_angle(p, number_of_points - 1, 0);
    if (angles_is_equal(std::abs(angle), M_PI)) {
        return CheckResult::ON_THE_EDGE;
    }
    full_angle += angle;
    if (angles_is_equal(std::abs(full_angle), 2 * M_PI)) {
        return CheckResult::INSIDE;
    } else if (angles_is_equal(std::abs(full_angle), 0)) {
        return CheckResult::OUTSIDE;
    } else {
        return CheckResult::ERROR;
    }
    return CheckResult::ERROR;
}
