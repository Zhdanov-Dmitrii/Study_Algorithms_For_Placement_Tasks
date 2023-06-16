#include "MeshTopology.hpp"

MeshTopology::MeshTopology(const std::string_view prefix, const std::string_view suffix, int hosts)
    : Topology(prefix, suffix, hosts) {}

bool MeshTopology::isFreeHostByNumHilbert(int i) {
    return isFreeHost(hilbertCurve[i]);
}

int MeshTopology::getHostNumByNumHilbert(int i) {
    return hilbertCurve[i];
}

NeighborTree MeshTopology::createNeighborTree(std::vector<int>& indHosts, const MeshTopology& topology) {
    NeighborTree res;
    res.root = std::make_shared<Neighbor>(createNeighbor(indHosts, 0, indHosts.size()));

    return res;
}

MeshTopology::Neighbor MeshTopology::createNeighbor(const std::vector<int> &indHosts, int l, int r) {
    Neighbor neighbor;
    neighbor.hostCount = r-l;
    if (r-l <= 2) {
        neighbor.isLeaf = true;
        for (int i = l; i < r; ++i)
            neighbor.hosts.push_back(indHosts[i]);

        return neighbor;
    }

    neighbor.isLeaf = false;
    neighbor.neighbors.push_back(std::make_shared<Neighbor>(createNeighbor(indHosts,l,(l+r)/2)));
    neighbor.neighbors.push_back(std::make_shared<Neighbor>(createNeighbor(indHosts,(l+r)/2,r)));

    return neighbor;
}