#include "TorusTaskManager.hpp"

TorusTaskManager::TorusTaskManager() : torusTopology(TorusTopology("torusNode-", "", 8, 8, 8)) {}

TorusTaskManager& TorusTaskManager::getInstance() {
    static TorusTaskManager instance;
    return instance;
}

void TorusTaskManager::createTasks(const Job &job) {
    switch (job.placementMode) {
        case PlacementMode::SIMPLE:
            createTaskInSimplePlacementMode(job, torusTopology);
            break;
        case PlacementMode::OPTIMAL:
            createTaskInOptimalPlacementMode(job, torusTopology);
            break;
        case PlacementMode::ADVANCED:
            createTaskInAdvancedPlacementMode(job, torusTopology);
            break;
        default:
            createTaskInRandomPlacementMode(job, torusTopology);
    }
}

void TorusTaskManager::createTaskInOptimalPlacementMode(Job job, Topology &topology) {

}

void TorusTaskManager::createTaskInAdvancedPlacementMode(Job job, Topology &topology) {

}



