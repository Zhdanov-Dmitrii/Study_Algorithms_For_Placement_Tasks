#include <iostream>
#include "TaskManager.hpp"

TaskManager::TaskManager() {
}

const std::unordered_map<unsigned long long int, std::shared_ptr<Job>> & TaskManager::getAllJobs() {
    return jobs;
}

const std::map<unsigned long long int, Task> & TaskManager::getAllTasks() {
    return tasks;
}

void TaskManager::updateTimeTask(const Task &task) {
    tasks[task.getId()].setTime(task.getTime());
    for (auto& [id, action] : task.getActions()) {
        tasks[task.getId()].getActions()[id].time = action.time;
    }
}

void TaskManager::createTaskInSimplePlacementMode(std::shared_ptr<Job>& jobPtr, Topology &topology) {
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);
    for(int i = 0; i <jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        tasks.push_back(std::move(task));
    }

    int indTask = 0;
    for(int i = 0; i < topology.getHosts() && indTask < jobPtr->processes; ++i) {
        if(topology.isFreeHost(i)) {
            tasks[indTask++].setHostName(topology.getHost(i));
            topology.removeFreeHost(i);
        }
    }
    if (indTask != jobPtr->processes) {
        throw std::out_of_range("There is no place to place the task in the topology");
    }

    fillActions(tasks);
    addTasks(tasks);
}

void TaskManager::createTaskInRandomPlacementMode(std::shared_ptr<Job>& jobPtr, Topology &topology) {
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);
    for(int i = 0; i <jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        tasks.push_back(std::move(task));
    }

    for (auto &task : tasks){
        if (topology.getFreeHosts().empty()) {
            std::cout << "no free hosts\n";
        }
        int indRandHost = topology.getRandomFreeHost();
        task.setHostName(topology.getHost(indRandHost));
        topology.removeFreeHost(indRandHost);
    }

    fillActions(tasks);
    addTasks(tasks);
}

void TaskManager::fillActions(std::vector<Task> &tasks) {
    if (tasks.empty()) return;

    auto& job = tasks[0].getJob();
    if (job->messages.empty() || job->messages[0].empty()){
        return;
    }
    if (job->jobType != JobType::CUBE) {
        for (int countMessage = job->messages[0][0].count; countMessage >= 0; --countMessage) {
            for (int i = 0; i < job->processes; ++i) {
                for (auto &msg: job->messages[i]) {
                    addAction(tasks, i, msg.process, msg.cost);
                }
            }
        }
    } else {
        for (int countMessage = job->messages[0][0].count; countMessage >- 0; -- countMessage) {
            auto cost = job->messages[0][0].cost;
            int n = cbrt(job->processes);
            for (int x = 0; x < n; ++x) {
                for (int y = 0; y < n; ++y) {
                    int z = 0;
                    auto getNum = [n](const Point3D& p) -> int { return p.x + n * p.y + n*n*p.z; };

                    ///По четным z
                    for (z = x % 2; z < n; z += 2) {
                        for (auto &msg: job->messages[getNum({x,y,z})]) {
                            addAction(tasks, getNum({x,y,z}), msg.process, msg.cost);
                        }
                    }

                    ///По нечетным z
                    for (z = x % 2 + 1; z < n; z += 2) {
                        for (auto &msg: job->messages[getNum({x,y,z})]) {
                            addAction(tasks, getNum({x,y,z}), msg.process, msg.cost);
                        }
                    }
                }
            }
        }
    }

}



//
//    switch (tasks[0].getJob()->jobType) {
//        case JobType::STAR:
//            fillActionsForStar(tasks);
//            break;
//        case JobType::GRID:
//            fillActionsForGrid(tasks);
//            break;
//        case JobType::CUBE:
//            fillActionsForCube(tasks);
//            break;
//        case JobType::TREE:
//            fillActionsForTree(tasks);
//            break;
//        default:
//            fillActionsForFull(tasks);
//    }
//}

void TaskManager::fillActionsForFull(std::vector<Task> &tasks) {
    auto& job = tasks[0].getJob();

    if (job->messages.empty() || job->messages[0].empty()){
        return;
    }

    int countMessage = job->messages[0][0].count;
    unsigned long cost = job->messages[0][0].cost;
    for (; countMessage >= 0; --countMessage) {

        for (int indTask = 0; indTask < job->processes; ++indTask) {
            for (int i = 0; i < job->processes; ++i) {
                if (i == indTask) continue;
                addAction(tasks, indTask, i, cost);
            }

            for (int i = 0; i < job->processes; ++i) {
                if (i == indTask) continue;
                addAction(tasks, i, indTask, cost);
            }
        }
    }


}

void TaskManager::fillActionsForStar(std::vector<Task> &tasks) {
    auto& job = tasks[0].getJob();

    if (job->messages.empty() || job->messages[0].empty()) {
        return;
    }
    int countMessage = job->messages[0][0].count;
    unsigned long cost = job->messages[0][0].cost;

    for (; countMessage >= 0; --countMessage) {
        ///Отправляем сообщение от мастер ноды
        for (int indTask = 1; indTask < job->processes; ++indTask) {
            addAction(tasks, 0, indTask, cost);
        }

        ///Мастер нода получает сообщения
        for (int indTask = 1; indTask < job->processes; ++indTask) {
            addAction(tasks, indTask,  0, cost);
        }
    }
}

void TaskManager::fillActionsForGrid(std::vector<Task> &tasks) {
    auto& job = tasks[0].getJob();

    if (job->messages.empty() || job->messages[0].empty()) {
        return;
    }

    ///Если между тасками будет разное кол-во сообщений, надо будет переписать
    int countMessage = job->messages[0][0].count;
    unsigned long cost = job->messages[0][0].cost;
    
    for (; countMessage >- 0; -- countMessage) {
        int n = sqrt(job->processes);
        for (int x = 0; x < n; ++x) {
            int y = 0;
            auto setAction = [x,y,n, cost, &tasks]
                    (Point2D (*func)(const Point2D &, const Point2D &, const Point2D &)) {
                auto getNum = [n](const Point2D& p) -> int { return p.x + n * p.y; };

                Point2D neighbor = func({x,y}, {n-1,n-1}, {0,0});
                TaskManager::addAction(tasks, getNum({x,y}), getNum(neighbor), cost);
            };

            ///По четным y
            for (y = x % 2; y < n; y += 2) {
                setAction(Point2dUtils::getUpPoint);
                setAction(Point2dUtils::getDownPoint);
                setAction(Point2dUtils::getLeftPoint);
                setAction(Point2dUtils::getRightPoint);
            }
            
            ///По нечетным y
            for (y = x % 2 + 1; y < n; y += 2) {
                setAction(Point2dUtils::getUpPoint);
                setAction(Point2dUtils::getDownPoint);
                setAction(Point2dUtils::getLeftPoint);
                setAction(Point2dUtils::getRightPoint);
            }
        }
    }
}

void TaskManager::fillActionsForCube(std::vector<Task> &tasks) {
    auto& job = tasks[0].getJob();

    if (job->messages.empty() || job->messages[0].empty()) {
        return;
    }

    ///Если между тасками будет разное кол-во сообщений, надо будет переписать
    int countMessage = job->messages[0][0].count;
    unsigned long cost = job->messages[0][0].cost;
    for (; countMessage >- 0; -- countMessage) {
        int n = cbrt(job->processes);
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                int z = 0;
                auto setAction = [x,y,z,n, cost, &tasks]
                        (Point3D (*func)(const Point3D &, const Point3D &, const Point3D &)) {
                    auto getNum = [n](const Point3D& p) -> int { return p.x + n * p.y + n*n*p.z; };

                    Point3D neighbor = func({x,y,z}, {n-1,n-1,n-1}, {0,0,0});
                    TaskManager::addAction(tasks, getNum({x,y,z}), getNum(neighbor), cost);
                };

                ///По четным z
                for (z = x % 2; z < n; z += 2) {
                    setAction(Point3dUtils::getUpPoint);
                    setAction(Point3dUtils::getDownPoint);
                    setAction(Point3dUtils::getLeftPoint);
                    setAction(Point3dUtils::getRightPoint);
                    setAction(Point3dUtils::getBehindPoint);
                    setAction(Point3dUtils::getBeforePoint);
                }

                ///По нечетным z
                for (z = x % 2 + 1; z < n; z += 2) {
                    setAction(Point3dUtils::getUpPoint);
                    setAction(Point3dUtils::getDownPoint);
                    setAction(Point3dUtils::getLeftPoint);
                    setAction(Point3dUtils::getRightPoint);
                    setAction(Point3dUtils::getBehindPoint);
                    setAction(Point3dUtils::getBeforePoint);
                }
            }
        }
    }
}

void TaskManager::fillActionsForTree(std::vector<Task> &tasks) {
    auto& job = tasks[0].getJob();

    if (job->messages.empty() || job->messages[0].empty()) {
        return;
    }
    int countMessage = job->messages[0][0].count;

    for (; countMessage >= 0; --countMessage) {
        fillTree(tasks, 0, true);
        fillTree(tasks, 0, false);
    }

}

void TaskManager::addTasks(std::vector<Task> &tasks) {
    for (auto& task : tasks) {
        this->tasks.emplace(task.getId(), std::move(task));
    }
}

void TaskManager::addAction(std::vector<Task> &tasks, int taskSender, int taskReciever, unsigned long cost) {
    tasks[taskSender].addAction({idAction++, ActionType::PUT, tasks[taskReciever].getHostName(), cost});
    tasks[taskReciever].addAction({idAction++, ActionType::GET, tasks[taskSender].getHostName(), cost});
}

void TaskManager::calcJobTime() {
    for (auto &[id, task] : tasks) {
        task.getJob()->time = std::max(task.getJob()->time, task.getTime());
    }
}

void TaskManager::removeInvalidJobs() {
    std::vector<int> remId;
    for(auto &[id, job] : jobs)
        if(!job->isValid)
            remId.push_back(id);

    for(auto id : remId)
        jobs.erase(id);

    remId.clear();
    for (auto &[id, task] : tasks)
        if(!task.getJob()->isValid)
            remId.push_back(id);

    for(auto id : remId)
        tasks.erase(id);

}

void TaskManager::fillTree(std::vector<Task> &tasks, int x, bool flag) {
    auto& job = tasks[0].getJob();

    auto getParent = [](int x) -> int { return x / 2 - 1 + x % 2; };
    auto getLeftChild = [](int x) -> int { return x * 2 + 1; };
    auto getRightChild = [](int x) -> int { return x * 2 + 2; };

    unsigned long cost = job->messages[0][0].cost;

    if (flag) {
        if (x != 0){
            addAction(tasks, x, getParent(x), cost);
        }

        if (getLeftChild(x) < job->processes) {
            addAction(tasks, x, getLeftChild(x), cost);
        }

        if (getRightChild(x) < job->processes) {
            addAction(tasks, x, getRightChild(x), cost);
        }
    }

    if (getLeftChild(x) < job->processes) {
        fillTree(tasks, getLeftChild(x), !flag);
    }
    if (getRightChild(x) < job->processes) {
        fillTree(tasks, getRightChild(x), !flag);
    }
}

void TaskManager::clear() {
    tasks.clear();
    jobs.clear();

    idAction = 0;
    idTask = 0;
}