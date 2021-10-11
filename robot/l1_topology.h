#pragma once

#include "topology.h"

class L1Topology : public Topology {
public:
    L1Topology(std::vector<std::vector<bool>> data, Point start) : Topology(data, start){};

    size_t MeasureDistance(const Point& from, const Point& to) const override;
};
