#pragma once

#include <cstddef>
#include <tuple>

struct Point {
    size_t x = 0;
    size_t y = 0;

    Point(size_t x = 0, size_t y = 0) : x(x), y(y) {
    }
    Point(const Point& from) = default;
    Point(Point&& from) = default;

    Point& operator=(const Point& from) {
        x = from.x;
        y = from.y;
        return *this;
    }
    Point& operator=(Point&& from) {
        x = from.x;
        y = from.y;
        return *this;
    }

    bool operator==(const Point& second) const {
        return x == second.x && y == second.y;
    }
    bool operator<(const Point& second) const {
        return std::tie(x, y) < std::tie(second.x, second.y);
    }
};
