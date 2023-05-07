#ifndef DIPLOMA_TOPOLOGYMANAGER_HPP
#define DIPLOMA_TOPOLOGYMANAGER_HPP

#include "../Domain/Topology/Topology.hpp"
#include "../Domain/Topology/TorusTopology.hpp"
#include "../Domain/Topology/TreeTopology.hpp"


class TopologyManager {
    /// Singleton
    TopologyManager();
    TopologyManager(const TopologyManager&);
    TopologyManager& operator=(TopologyManager&);

    TorusTopology torus;
    TreeTopology fatTree;
    TreeTopology thinTree;
public:
    enum TopologyType {
        TORUS,
        FAT_TREE,
        THIN_TREE
    };

    static TopologyManager& getInstance();
    TorusTopology getTorusTopology();
    TreeTopology getFatTreeTopology();
    TreeTopology getThinTreeTopology();
};


#endif //DIPLOMA_TOPOLOGYMANAGER_HPP
