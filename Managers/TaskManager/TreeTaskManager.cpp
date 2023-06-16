#include "TreeTaskManager.hpp"

TreeTaskManager& TreeTaskManager::getInstance() {
    static TreeTaskManager instance;
    return instance;
}

TreeTaskManager::TreeTaskManager() {}

void TreeTaskManager::createTasks(Job job, Topology& topology) {
    auto jobPtr = std::make_shared<Job>(std::move(job));
    jobs.emplace(jobPtr->id, jobPtr);

    switch (jobPtr->placementMode) {
        case PlacementMode::SIMPLE:
            createTaskInSimplePlacementMode(jobPtr, topology);
            break;
        case PlacementMode::OPTIMAL:
            createTaskInOptimalPlacementMode(jobPtr, topology);
            break;
        case PlacementMode::ADVANCED:
            createTaskInAdvancedPlacementMode(jobPtr, topology);
            break;
        default:
            createTaskInRandomPlacementMode(jobPtr, topology);
    }
}

void TreeTaskManager::createTaskInOptimalPlacementMode(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);

    std::vector<int> indHosts = searchHostForOptimalPlacement(jobPtr, topology);

    auto& tree = dynamic_cast<TreeTopology &>(topology);

    for(int i = 0; i < jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        task.setHostName(tree.getHost(indHosts[i]));
        tasks.push_back(std::move(task));

        tree.removeFreeHost(indHosts[i]);
    }

    fillActions(tasks);
    addTasks(tasks);
}

void TreeTaskManager::createTaskInAdvancedPlacementMode(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<int> indHost(jobPtr->processes);
    NeighborTree neighborTree = searchNeighborTree(jobPtr, topology);
    mappingJob(jobPtr, neighborTree, indHost);

    auto& tree = dynamic_cast<TreeTopology &>(topology);

    std::vector<Task> tasks;
    for(int i = 0; i <jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        task.setHostName(tree.getHost(indHost[i]));

        tasks.push_back(std::move(task));

        tree.removeFreeHost(indHost[i]);
    }

    fillActions(tasks);
    addTasks(tasks);
}

std::vector<int> TreeTaskManager::searchHostForOptimalPlacement(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<int> indHosts;
    indHosts.reserve(jobPtr->processes);

    auto& tree = dynamic_cast<TreeTopology &>(topology);

    bool f = true;
    int indTask = 0;
    for (int indLvl = 0; indLvl < tree.getCountLevels() && f; ++indLvl) {
        auto& level = tree.getLevel(indLvl);
        for (int indSw = 0; indSw < level.size() && f; ++indSw) {
            if (level[indSw]->freeNodes.size() < jobPtr->processes) continue;

            searchHost(level[indSw], jobPtr->processes, indTask, tree, indHosts);
            f = false;
        }
    }

    if (indTask != jobPtr->processes) {
        throw std::out_of_range("There is no place to searchHost the task in the tree treeTopology");
    }

    return indHosts;
}

void TreeTaskManager::searchHost(std::weak_ptr<Switch> sw, int nodes, int &indTask, TreeTopology& tree, std::vector<int>& indHosts) {
    if (sw.lock()->down.empty()) {
        for (int freeHost : sw.lock()->freeNodes) {
            if (--nodes < 0) break;

            indHosts[indTask++] =freeHost;
        }
        return;
    }

    using SwitchPtr = std::shared_ptr<Switch>;
    std::vector<SwitchPtr> sorted = sw.lock()->down;
    std::sort(sorted.begin(), sorted.end(), [] (const SwitchPtr& a, const SwitchPtr& b) {
        return a->freeNodes.size() > b->freeNodes.size();
    });

    for (auto &child : sorted) {
        if (nodes <= 0)
            return;

        int count = child->freeNodes.size();
        searchHost(child, std::min(count, nodes), indTask, tree, indHosts);
        nodes -= std::min(count, nodes);
    }

}

NeighborTree TreeTaskManager::searchNeighborTree(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    NeighborTree res;

    auto& tree = dynamic_cast<TreeTopology &>(topology);

    bool f = true;
    for (int indLvl = 0; indLvl < tree.getCountLevels() && f; ++indLvl) {
        auto& level = tree.getLevel(indLvl);
        for (int indSw = 0; indSw < level.size() && f; ++indSw) {
            if (level[indSw]->freeNodes.size() < jobPtr->processes) continue;

            res.root = std::make_shared<Neighbor>(searchNeighbor(level[indSw], jobPtr->processes, tree));
            f = false;
        }
    }

    return res;
}

TreeTaskManager::Neighbor TreeTaskManager::searchNeighbor(std::weak_ptr<Switch> sw, int nodes, TreeTopology& tree) {
    Neighbor res;
    res.hostCount = nodes;

    if (sw.lock()->down.empty()) {
        res.isLeaf = true;
        for (int freeHost : sw.lock()->freeNodes) {
            if (--nodes < 0) break;
            res.hosts.push_back(freeHost);
        }
        return res;
    }
    res.isLeaf = false;

    using SwitchPtr = std::shared_ptr<Switch>;
    std::vector<SwitchPtr> sorted = sw.lock()->down;
    std::sort(sorted.begin(), sorted.end(), [] (const SwitchPtr& a, const SwitchPtr& b) {
        return a->freeNodes.size() > b->freeNodes.size();
    });

    for (auto &child : sorted) {
        if (nodes <= 0)
            break;

        int count = child->freeNodes.size();
        auto neighbor = std::make_shared<Neighbor>(searchNeighbor(child, std::min(count, nodes), tree));
        res.neighbors.push_back(neighbor);
        nodes -= std::min(count, nodes);
    }

    return res;
}


