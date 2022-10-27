#pragma once

#include <string>

struct Point {
    double x;
    double y;
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
        OUTSIDE,
        INSIDE,
        ON_THE_EDGE,
    };
    Polygon();
    ~Polygon();
    void read_file(const std::string& filename);
    CheckResult where_point(const Point& p) const;
};
