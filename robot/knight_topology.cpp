#include "knight_topology.h"

std::vector<Point> KnightTopology::Lookup() const {
    std::vector<Point> neighbours;
    for (const auto& [dx, dy] : std::vector<std::pair<ssize_t, ssize_t>>{
             {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}}) {
        ssize_t nx = current_.x + dx, ny = current_.y + dy;
        if (nx < 0 || ny < 0) {
            continue;
        }
        Point neighbour({static_cast<size_t>(nx), static_cast<size_t>(ny)});

        if (IsFree(neighbour)) {
            neighbours.push_back(neighbour);
        }
    }

    return neighbours;
}
