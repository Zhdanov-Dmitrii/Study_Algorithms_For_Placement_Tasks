#ifndef DIPLOMA_TORUSTASKMANAGER_H
#define DIPLOMA_TORUSTASKMANAGER_H


#include "../../Domain/Topology/TorusTopology.hpp"
#include "TaskManager.hpp"

class TorusTaskManager : public TaskManager {
    ///Singleton
    TorusTaskManager();
    TorusTaskManager(TorusTaskManager&) = default;
    TorusTaskManager& operator=(TorusTaskManager&) = default;

    TorusTopology torusTopology;
protected:
    void createTaskInOptimalPlacementMode(Job job, Topology& topology) override;
    void createTaskInAdvancedPlacementMode(Job job, Topology& topology) override;

public:
    static TorusTaskManager& getInstance();

    virtual void createTasks(const Job& job) override;
};


#endif //DIPLOMA_TORUSTASKMANAGER_H
