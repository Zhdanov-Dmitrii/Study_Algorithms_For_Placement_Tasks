//
// Created by user on 18.04.23.
//

#include "TopologyManager.hpp"

TopologyManager &TopologyManager::getInstance() {
    static TopologyManager instance;
    return instance;
}

TopologyManager::TopologyManager():
    torus(TorusTopology("torusNode-", "", 16, 16, 16)),
    fatTree(TreeTopology("fatTree-", "", 4096, 6, 4, 4)),
    thinTree(TreeTopology("thinTree-","",4096, 6, 4, 2)){}

TorusTopology TopologyManager::getTorusTopology() {
    return torus;
}

TreeTopology TopologyManager::getFatTreeTopology() {
    return fatTree;
}

TreeTopology TopologyManager::getThinTreeTopology() {
    return thinTree;
}