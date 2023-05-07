#include "SimGridManager.hpp"

SimGridManager::SimGridManager()
    : torusTaskManager(TorusTaskManager::getInstance()) {
    engine->load_platform("../platform.xml");
}

void SimGridManager::registerTasks() {
    for (auto& [id, task] : TaskManager::getAllTasks()) {
        simgrid::s4u::Actor::create(task.getHostName(), engine->host_by_name(task.getHostName()), task);
    }
}

void SimGridManager::run() {
    engine->run();
}

SimGridManager& SimGridManager::getInstance() {
    static SimGridManager instance;
    return instance;
}

void SimGridManager::killAllActor() {
    simgrid::s4u::Actor::kill_all();
}



