#ifndef DIPLOMA_TREETASKMANAGER_H
#define DIPLOMA_TREETASKMANAGER_H


#include "../../Domain/Topology/Topology.hpp"
#include "../../Domain/Topology/TreeTopology.hpp"
#include "TaskManager.hpp"

class TreeTaskManager : public TaskManager{
    ///Singleton
    TreeTaskManager();
    TreeTaskManager(const TreeTaskManager&) = default;
    TreeTaskManager& operator=(TreeTaskManager&) = default;

    using Switch = TreeTopology::Switch;

    void place(std::weak_ptr<Switch> sw, int nodes, int &indTask, TreeTopology& tree, std::vector<Task>& tasks);

protected:
    void createTaskInOptimalPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
    void createTaskInAdvancedPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
public:
    static TreeTaskManager& getInstance();
    void createTasks(Job job, Topology& topology) override;
};


#endif //DIPLOMA_TREETASKMANAGER_H
