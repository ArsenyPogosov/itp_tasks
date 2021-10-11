#pragma once

#include "point.h"
#include "field.h"

#include <vector>

class Topology : public Field {
public:
    Topology(std::vector<std::vector<bool>> data, const Point& start = {0, 0}) : Field(data), current_(start){};

    virtual std::vector<Point> Lookup() const = 0;
    virtual size_t MeasureDistance(const Point& from, const Point& to) const = 0;
    virtual void Move(const Point& point) {
        current_ = point;
    };

    const Point& Current() const {
        return current_;
    };

protected:
    Point current_;
};
