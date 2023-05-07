#include "TorusTaskManager.hpp"

TorusTaskManager::TorusTaskManager() : torusTopology(TorusTopology("torusNode-", "", 16, 16, 16)) {}

TorusTaskManager& TorusTaskManager::getInstance() {
    static TorusTaskManager instance;
    return instance;
}

void TorusTaskManager::createTasks(Job job) {
    auto jobPtr = std::make_shared<Job>(std::move(job));
    jobs.emplace(jobPtr->id, jobPtr);

    switch (jobPtr->placementMode) {
        case PlacementMode::SIMPLE:
            createTaskInSimplePlacementMode(jobPtr, torusTopology);
            break;
        case PlacementMode::OPTIMAL:
            createTaskInOptimalPlacementMode(jobPtr, torusTopology);
            break;
        case PlacementMode::ADVANCED:
            createTaskInAdvancedPlacementMode(jobPtr, torusTopology);
            break;
        default:
            createTaskInRandomPlacementMode(jobPtr, torusTopology);
    }
}

void TorusTaskManager::createTaskInOptimalPlacementMode(std::shared_ptr<Job>& jobPtr, Topology &topology) {
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);
    for(int i = 0; i <jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        tasks.push_back(std::move(task));
    }

    auto& torus = dynamic_cast<TorusTopology &>(topology); //У нас гарантировано приходит топология тора

    std::vector<std::pair<int,int>> freeSegments; //стек сегментов [длина, индекс начала]
    int currCount = 0;
    int start = 0;
    bool flag = false; //Для вставки последнего элемента
    for (int i = 0; i < torus.getHosts(); ++i) {
        if (!torus.isFreeHostByNumHilbert(i)) {
            if (currCount != 0) {
                freeSegments.emplace_back(currCount, start);
            }
            start = i+1;
            currCount = 0;
            flag = false;
        } else {
            ++currCount;
            flag = true;
        }
    }

    if(flag) {
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
    int indTask = 0;
    while (processes > 0 && resI+1 < int(freeSegments.size())) {
        ++resI;
        auto seg = freeSegments[resI];
        for (int i = 0; i < seg.first && processes > 0; ++i) {
            --processes;
            tasks[indTask++].setHostName(torus.getHostByNumHilbert(seg.second+i));
            torus.removeFreeHost(torus.getHostNumByNumHilbert(seg.second+i));
        }

    }

//    std::sort(freeSegments.begin(), freeSegments.end());
//
//    int indTask = 0;
//    int processes = jobPtr->processes;
//    while (processes) {
//        if (freeSegments.empty())
//            throw std::out_of_range("no free hosts in torus topology");
//
//        auto itSegment = std::upper_bound(freeSegments.begin(), freeSegments.end(), std::make_pair(processes, 0),
//                                           [](const std::pair<int,int>&a, const std::pair<int,int>&b){
//            return a.first < b.first;
//        });
//        if (itSegment == freeSegments.end()) --itSegment;
//
//        auto [count, ind] = *itSegment;
//        while (processes && count) {
//            tasks[indTask].setHostName(torus.getHostByNumHilbert(ind));
//            torus.removeFreeHost(torus.getHostNumByNumHilbert(ind));
//
//            ++ind;
//            ++indTask;
//            --count;
//            --processes;
//        }
//
//        freeSegments.erase(itSegment);
//    }

    fillActions(tasks);
    addTasks(tasks);
}

void TorusTaskManager::createTaskInAdvancedPlacementMode(std::shared_ptr<Job>& jobPtr, Topology &topology) {

}

void TorusTaskManager::clear() {
    tasks.clear();
    jobs.clear();

    idAction = 0;
    idTask = 0;

    torusTopology = TorusTopology("torusNode-", "", 16, 16, 16);
}



