#include "l1_topology.h"

size_t L1Topology::MeasureDistance(const Point& from, const Point& to) const {
    return std::max(from.x, to.x) + std::max(from.y, to.y) - std::min(from.x, to.x) - std::min(from.y, to.y);
}
