#pragma once

#include "l1_topology.h"

class CheckersTopology : public L1Topology {
public:
    CheckersTopology(std::vector<std::vector<bool>> data, Point start = {0, 0}) : L1Topology(data, start){};

    std::vector<Point> Lookup() const override;
};
