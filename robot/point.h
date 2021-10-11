#pragma once

#include <cstddef>
#include <tuple>

struct Point {
    size_t x = 0;
    size_t y = 0;
    
    bool operator==(const Point& second) const {
        return x == second.x && y == second.y;
    }
    bool operator<(const Point& second) const {
        return std::tie(x, y) < std::tie(second.x, second.y);
    }
};
