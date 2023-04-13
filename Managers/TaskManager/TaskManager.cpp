#include "TaskManager.hpp"

TaskManager::TaskManager() : simGridManager(SimGridManager::getInstance()) {
}

const std::unordered_map<int, Task> &TaskManager::getAll() {
    return data;
}

void TaskManager::updateTimeTask(const Task &task) {
    data[task.getId()].setTime(task.getTime());
    for (auto& [id, action] : task.getActions()) {
        data[task.getId()].getActions()[id].time = action.time;
    }
}

void TaskManager::createTaskInSimplePlacementMode(Job job, Topology &topology) {
    auto jobPtr = std::make_shared<Job>(std::move(job));
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

void TaskManager::createTaskInRandomPlacementMode(Job job, Topology &topology) {
    auto jobPtr = std::make_shared<Job>(std::move(job));
    std::vector<Task> tasks;
    tasks.reserve(jobPtr->processes);
    for(int i = 0; i <jobPtr->processes; ++i) {
        Task task(idTask++);
        task.setJob(jobPtr);
        tasks.push_back(std::move(task));
    }

    for (auto &task : tasks){
        int indRandHost = topology.getRandomFreeHost();
        task.setHostName(topology.getHost(indRandHost));
        topology.removeFreeHost(indRandHost);
    }

    fillActions(tasks);
    addTasks(tasks);
}

void TaskManager::fillActions(std::vector<Task> &tasks) {
    if (tasks.empty()) return;

    switch (tasks[0].getJob()->jobType) {
        case JobType::STAR:
            fillActionsForStar(tasks);
            break;
        case JobType::GRID:
            fillActionsForGrid(tasks);
            break;
        case JobType::CUBE:
            fillActionsForCube(tasks);
            break;
        case JobType::TREE:
            fillActionsForTree(tasks);
            break;
        default:
            fillActionsForFull(tasks);
    }
}

void TaskManager::fillActionsForFull(std::vector<Task> &tasks) {
    //todo: Сложно, думать надо...
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
                int z;
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

}

void TaskManager::addTasks(std::vector<Task> &tasks) {
    for (auto& task : tasks) {
        data.emplace(task.getId(), std::move(task));
    }
}

void TaskManager::addAction(std::vector<Task> &tasks, int taskSender, int taskReciever, unsigned long cost) {
    tasks[taskSender].addAction({idAction++, ActionType::PUT, tasks[taskReciever].getHostName(), cost});
    tasks[taskReciever].addAction({idAction++, ActionType::GET, tasks[taskSender].getHostName(), cost});
}