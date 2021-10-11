#include "robot.h"

#include <set>
#include <algorithm>

bool DFSPathExists(Topology& topology, const Point& start, const Point& finish, std::set<Point>& used) {
    if (start == finish) {
        return true;
    }

    if (used.find(start) != used.end()) {
        return false;
    }
    used.insert(start);

    topology.Move(start);
    std::vector<Point> neighbours = topology.Lookup();
    std::sort(neighbours.begin(), neighbours.end(), [&](Point a, Point b) {
        return topology.MeasureDistance(a, finish) < topology.MeasureDistance(b, finish);
    });

    for (const auto& neighbour : neighbours) {
        if (DFSPathExists(topology, neighbour, finish, used)) {
            return true;
        }
    }

    used.erase(start);
    return false;
}

bool PathExists(Topology& topology, const Point& finish) {
    std::set<Point> used;
    return DFSPathExists(topology, topology.Current(), finish, used);
}
