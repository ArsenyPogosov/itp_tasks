#include "overflow_topology.h"

std::vector<Point> OverflowTopology::Lookup() const {
    std::vector<Point> neighbours;
    for (const auto& [dx, dy] : std::vector<std::pair<ssize_t, ssize_t>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
        ssize_t nx = (current_.x + dx + Width()) % Width(), ny = (current_.y + dy + Height()) % Height();
        Point neighbour(static_cast<size_t>(nx), static_cast<size_t>(ny));

        if (IsFree(neighbour)) {
            neighbours.push_back(neighbour);
        }
    }

    return neighbours;
}
