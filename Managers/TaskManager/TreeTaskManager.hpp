#ifndef DIPLOMA_TREETASKMANAGER_H
#define DIPLOMA_TREETASKMANAGER_H


#include "../../Domain/Topology/Topology.hpp"
#include "../../Domain/Topology/TreeTopology.hpp"
#include "../../Domain/NeighborTree.hpp"
#include "TaskManager.hpp"

class TreeTaskManager : public TaskManager{
    ///Singleton
    TreeTaskManager();
    TreeTaskManager(const TreeTaskManager&) = default;
    TreeTaskManager& operator=(TreeTaskManager&) = default;

    using Switch = TreeTopology::Switch;
    using Neighbor = NeighborTree::Neighbor;

protected:
    void createTaskInOptimalPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
    void createTaskInAdvancedPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
    std::vector<int> searchHostForOptimalPlacement(std::shared_ptr<Job>& jobPtr, Topology& topology) override;
    void searchHost(std::weak_ptr<Switch> sw, int nodes, int &indTask, TreeTopology& tree, std::vector<int>& indHosts);

    NeighborTree searchNeighborTree(std::shared_ptr<Job>& jobPtr, Topology& topology);
    Neighbor searchNeighbor(std::weak_ptr<Switch> sw, int nodes, TreeTopology& tree);
public:
    static TreeTaskManager& getInstance();
    void createTasks(Job job, Topology& topology) override;
};


#endif //DIPLOMA_TREETASKMANAGER_H
