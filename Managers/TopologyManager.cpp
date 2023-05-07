//
// Created by user on 18.04.23.
//

#include "TopologyManager.hpp"

TopologyManager &TopologyManager::getInstance() {
    static TopologyManager instance;
    return instance;
}

TorusTopology TopologyManager::getTorusTopology() {
    return {"torusNode-", "", 16, 16, 16};
}

TreeTopology TopologyManager::getFatTreeTopology() {
    return {"fatTree-", "", 4096, 6, 4, 4};
}

TreeTopology TopologyManager::getThinTreeTopology() {
    return {"thinTree-","",4096, 6, 4, 2};
}