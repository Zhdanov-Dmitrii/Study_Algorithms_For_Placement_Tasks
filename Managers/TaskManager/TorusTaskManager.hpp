#ifndef DIPLOMA_TORUSTASKMANAGER_H
#define DIPLOMA_TORUSTASKMANAGER_H


#include <list>
#include <algorithm>

#include "../../Domain/Topology/TorusTopology.hpp"
#include "TaskManager.hpp"

class TorusTaskManager : public TaskManager {
    ///Singleton
    TorusTaskManager();
    TorusTaskManager(TorusTaskManager&) = default;
    TorusTaskManager& operator=(TorusTaskManager&) = default;
protected:
    void createTaskInOptimalPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
    void createTaskInAdvancedPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) override;

    std::vector<int> searchHostForOptimalPlacement(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
public:
    static TorusTaskManager& getInstance();

    void createTasks(Job job, Topology& topology) override;
};


#endif //DIPLOMA_TORUSTASKMANAGER_H
