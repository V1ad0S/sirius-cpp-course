#pragma once

#include <string>

struct Point {
    double x;
    double y;
    Point operator-(const Point& other);
};

class Polygon {
   private:
    Point* points;
    size_t number_of_points;
    int check_file(const std::string& filename) const;
    double calculate_angle(const Point& p, size_t p1_index, size_t p2_index) const;
    bool is_point_on_edge(const Point& p, size_t p1_index, size_t p2_index) const;

   public:
    enum class CheckResult {
        ERROR = -1,
        OUTSIDE = 0,
        INSIDE = 1,
        ON_THE_EDGE = 2,
    };
    Polygon();
    ~Polygon();
    void read_file(const std::string& filename);
    CheckResult where_point(const Point& p) const;
};
