#ifndef DIPLOMA_TOPOLOGYMANAGER_HPP
#define DIPLOMA_TOPOLOGYMANAGER_HPP

#include "../Domain/Topology/Topology.hpp"
#include "../Domain/Topology/TorusTopology.hpp"
#include "../Domain/Topology/TreeTopology.hpp"
#include "../Domain/Topology/GridTopology.hpp"


class TopologyManager {
    /// Singleton
    TopologyManager();
    TopologyManager(const TopologyManager&);
    TopologyManager& operator=(TopologyManager&) = delete;

    TorusTopology torus;
    GridTopology grid;
    TreeTopology fatTree;
    TreeTopology thinTree;
public:
    enum TopologyType {
        TORUS_TOPOLOGY,
        GRID_TOPOLOGY,
        FAT_TREE_TOPOLOGY,
        THIN_TREE_TOPOLOGY
    };

    static TopologyManager& getInstance();
    GridTopology getGridTopology();
    TorusTopology getTorusTopology();
    TreeTopology getFatTreeTopology();
    TreeTopology getThinTreeTopology();
};


#endif //DIPLOMA_TOPOLOGYMANAGER_HPP
