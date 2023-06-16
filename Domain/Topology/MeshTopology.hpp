#ifndef DIPLOMA_MESHTOPOLOGY_HPP
#define DIPLOMA_MESHTOPOLOGY_HPP

#include "Topology.hpp"
#include "../NeighborTree.hpp"

class MeshTopology : public Topology {
    using Neighbor = NeighborTree::Neighbor;

    static Neighbor createNeighbor(const std::vector<int>& indHosts, int l, int r);
protected:
    MeshTopology(const std::string_view prefix, const std::string_view suffix, int hosts);
    std::vector<int> hilbertCurve;

public:


    bool isFreeHostByNumHilbert(int i);
    int getHostNumByNumHilbert(int i);

    static NeighborTree createNeighborTree(std::vector<int>& indHosts, const MeshTopology& topology);
};


#endif //DIPLOMA_MESHTOPOLOGY_HPP
