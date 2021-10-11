#pragma once

#include <point.h>

#include <vector>

class Field {
public:
    Field(std::vector<std::vector<bool>> data);
    bool IsFree(Point p) const;

    size_t Height() const;
    size_t Width() const;

private:
    std::vector<std::vector<bool>> data_;
};
