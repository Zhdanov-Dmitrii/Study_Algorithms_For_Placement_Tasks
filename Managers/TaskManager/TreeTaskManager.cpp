#include "TreeTaskManager.hpp"

TreeTaskManager& TreeTaskManager::getInstance() {
    static TreeTaskManager instance;
    return instance;
}

TreeTaskManager::TreeTaskManager() {}

void TreeTaskManager::createTaskInOptimalPlacementMode(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);
    for(int i = 0; i < jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        tasks.push_back(std::move(task));
    }

    auto& tree = dynamic_cast<TreeTopology &>(topology);


    bool f = true;
    int indTask = 0;
    for (int indLvl = 0; indLvl < tree.getCountLevels() && f; ++indLvl) {
        auto& level = tree.getLevel(indLvl);
        for (int indSw = 0; indSw < level.size() && f; ++indSw) {
            if (level[indSw]->freeNodes.size() < jobPtr->processes) continue;

            if (jobPtr->processes == 256) {
                int i =0;
                ++i;
            }
            place(level[indSw], jobPtr->processes, indTask, tree, tasks);
//            std::vector<int> deletedFreeHosts;
//            deletedFreeHosts.reserve(jobPtr->processes);
//            for (int freeHost : level[indSw]->freeNodes) {
//                if (indTask >= tasks.size()) break;
//
//                tasks[indTask++].setHostName(tree.getHost(freeHost));
//                deletedFreeHosts.push_back(freeHost);
//            }
//
//            for (auto host : deletedFreeHosts) {
//                tree.removeFreeHost(host);
//            }


            f = false;
        }
    }

    if (indTask != jobPtr->processes) {
        throw std::out_of_range("There is no place to place the task in the tree treeTopology");
    }

    fillActions(tasks);
    addTasks(tasks);
}

void TreeTaskManager::createTaskInAdvancedPlacementMode(std::shared_ptr<Job> &jobPtr, Topology &topology) {

}

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

void TreeTaskManager::place(std::weak_ptr<Switch> sw, int nodes, int &indTask, TreeTopology& tree, std::vector<Task>& tasks) {
    if (sw.lock()->down.empty()) {
        std::vector<int> deletedFreeHosts;
        for (int freeHost : sw.lock()->freeNodes) {
            --nodes;
            if (nodes < 0)
                break;

            tasks[indTask++].setHostName(tree.getHost(freeHost));
            deletedFreeHosts.push_back(freeHost);
        }

        for (auto host : deletedFreeHosts) {
            tree.removeFreeHost(host);
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
        place(child, std::min(count, nodes), indTask, tree, tasks);
        nodes -= std::min(count, nodes);
    }

}
