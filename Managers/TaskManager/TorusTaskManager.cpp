#include "TorusTaskManager.hpp"
#include <iostream>
#include <random>

TorusTaskManager::TorusTaskManager() {}

TorusTaskManager &TorusTaskManager::getInstance() {
    static TorusTaskManager instance;
    return instance;
}

void TorusTaskManager::createTasks(Job job, Topology &topology) {
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

void TorusTaskManager::createTaskInOptimalPlacementMode(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<int> indHost = searchHostForOptimalPlacement(jobPtr, topology);
    std::shuffle(indHost.begin(), indHost.begin() + indHost.size() / 2, std::mt19937(std::random_device()()));

    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);

    auto &mesh = dynamic_cast<MeshTopology &>(topology); //У нас гарантировано приходит сетчатая топология

    for (int i = 0; i < jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        task.setHostName(mesh.getHost(indHost[i]));

        tasks.push_back(std::move(task));

        mesh.removeFreeHost(indHost[i]);
    }

    fillActions(tasks);
    addTasks(tasks);
}

void TorusTaskManager::createTaskInAdvancedPlacementMode(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<int> indHost = searchHostForOptimalPlacement(jobPtr, topology);
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);

    auto &mesh = dynamic_cast<MeshTopology &>(topology); //У нас гарантировано приходит топология тора

    NeighborTree neighborTree = MeshTopology::createNeighborTree(indHost, mesh);
    mappingJob(jobPtr, neighborTree, indHost);

    for (int i = 0; i < jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        task.setHostName(mesh.getHost(indHost[i]));

        tasks.push_back(std::move(task));

        mesh.removeFreeHost(indHost[i]);
    }

    fillActions(tasks);
    addTasks(tasks);
}

std::vector<int> TorusTaskManager::searchHostForOptimalPlacement(std::shared_ptr<Job> &jobPtr, Topology &topology) {
    std::vector<int> indHosts;
    indHosts.reserve(jobPtr->processes);

    auto &mesh = dynamic_cast<MeshTopology &>(topology); //У нас гарантировано приходит топология тора

    std::vector<std::pair<int, int>> freeSegments; //стек сегментов [длина, индекс начала]
    int currCount = 0;
    int start = 0;
    bool flag = false; //Для вставки последнего элемента
    for (int i = 0; i < mesh.getHosts(); ++i) {
        if (!mesh.isFreeHostByNumHilbert(i)) {
            if (currCount != 0) {
                freeSegments.emplace_back(currCount, start);
            }
            start = i + 1;
            currCount = 0;
            flag = false;
        } else {
            ++currCount;
            flag = true;
        }
    }

    if (flag) {
        freeSegments.emplace_back(currCount, start);
    }

    int minLen = std::numeric_limits<int>::max();
    int resI = -1;
    int processes = jobPtr->processes;
    for (int i = 0; i < freeSegments.size(); ++i) {
        int len = -1;
        int tempI = i;
        int tempProcesses = processes;
        while (tempProcesses >= 0 && tempI < freeSegments.size()) {
            if (freeSegments[tempI].first >= tempProcesses) {
                len = freeSegments[tempI].second + tempProcesses - freeSegments[i].second;
                break;
            }

            tempProcesses -= freeSegments[tempI].first;
            ++tempI;
        }

        if (len == -1)
            break;

        if (len < minLen) {
            minLen = len;
            resI = i;
        }
    }

    if (resI == -1) {
        throw std::runtime_error("No free hosts in torus");
    }

    if (!jobPtr->path.empty()) {
        --resI;
        ++resI;
    }
    --resI;
    while (processes > 0 && resI + 1 < int(freeSegments.size())) {
        ++resI;
        auto seg = freeSegments[resI];
        for (int i = 0; i < seg.first && processes > 0; ++i) {
            --processes;
            indHosts.push_back(mesh.getHostNumByNumHilbert(seg.second + i));
        }
    }

    return indHosts;
}

