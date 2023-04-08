#ifndef DIPLOMA_SIMGRIDMANAGER_H
#define DIPLOMA_SIMGRIDMANAGER_H

#include <simgrid/s4u.hpp>
#include "TaskManager/TaskManager.hpp"
#include "../Domain/Task.hpp"

class SimGridManager {
    ///Singleton
    SimGridManager();
    SimGridManager(SimGridManager&);
    SimGridManager& operator=(SimGridManager&);

    simgrid::s4u::Engine* engine = simgrid::s4u::Engine::get_instance();
public:
    static SimGridManager& getInstance();

    void registerTasks();

    void run();

    void calcResult();
};


#endif //DIPLOMA_SIMGRIDMANAGER_H
