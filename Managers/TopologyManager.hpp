#ifndef DIPLOMA_TOPOLOGYMANAGER_HPP
#define DIPLOMA_TOPOLOGYMANAGER_HPP

#include "../Domain/Topology/Topology.hpp"
#include "../Domain/Topology/TorusTopology.hpp"
#include "../Domain/Topology/TreeTopology.hpp"


class TopologyManager {
    /// Singleton
    TopologyManager() = default;
    TopologyManager(const TopologyManager&);
    TopologyManager& operator=(TopologyManager&) = delete;
public:
    enum TopologyType {
        TORUS_TOPOLOGY,
        FAT_TREE_TOPOLOGY,
        THIN_TREE_TOPOLOGY
    };

    static TopologyManager& getInstance();
    TorusTopology getTorusTopology();
    TreeTopology getFatTreeTopology();
    TreeTopology getThinTreeTopology();
};


#endif //DIPLOMA_TOPOLOGYMANAGER_HPP
