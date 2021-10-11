#pragma once

#include "l1_topology.h"

class PlanarTopology : public L1Topology {
public:
    PlanarTopology(std::vector<std::vector<bool>> data, Point start = {0, 0}) : L1Topology(data, start){};

    std::vector<Point> Lookup() const override;
};
