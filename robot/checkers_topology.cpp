#include "checkers_topology.h"

#include <set>
#include <queue>

std::vector<Point> CheckersTopology::Lookup() const {
    std::vector<Point> neighbours;

    for (const auto& [dx, dy] : std::vector<std::pair<ssize_t, ssize_t>>{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}) {
        ssize_t nx = current_.x + dx, ny = current_.y + dy;
        if (nx < 0 || ny < 0) {
            continue;
        }
        Point neighbour(static_cast<size_t>(nx), static_cast<size_t>(ny));

        if (IsFree(neighbour)) {
            neighbours.push_back(neighbour);
        }
    }

    std::set<Point> used;
    std::queue<Point> q;
    q.push(current_);
    used.insert(current_);

    const auto add_point = [&](const Point& p) {
        if (used.find(p) == used.end()) {
            used.insert(p);
            neighbours.push_back(p);
            q.push(p);
        }
    };

    while (!q.empty()) {
        Point curr = q.front();
        q.pop();

        for (const auto& [dx, dy] : std::vector<std::pair<ssize_t, ssize_t>>{{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}) {
            ssize_t nx = curr.x + dx * 2, ny = curr.y + dy * 2;
            if (nx < 0 || ny < 0) {
                continue;
            }
            Point neighbour(static_cast<size_t>(nx), static_cast<size_t>(ny));

            if (IsFree(neighbour)) {
                add_point(neighbour);
            }
        }
    }

    return neighbours;
}
