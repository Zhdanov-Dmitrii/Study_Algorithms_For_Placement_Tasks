#include "SimGridManager.hpp"

SimGridManager::SimGridManager() {
    engine->load_platform("../platform.xml");
}

void SimGridManager::registerTasks() {
    for (auto& [id, task] : TaskManager::getAll()) {
        simgrid::s4u::Actor::create(task.getHostName(), engine->host_by_name(task.getHostName()), task);
    }
}

void SimGridManager::run() {
    engine->run();
}

void SimGridManager::calcResult() {
    for(auto& actor : engine->get_all_actors()) {

    }
}

SimGridManager& SimGridManager::getInstance() {
    static SimGridManager instance;
    return instance;
}



