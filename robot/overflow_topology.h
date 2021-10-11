#pragma once

#include "l1_topology.h"

class OverflowTopology : public L1Topology {
public:
    OverflowTopology(std::vector<std::vector<bool>> data, Point start = {0, 0}) : L1Topology(data, start){};

    std::vector<Point> Lookup() const override;
};
