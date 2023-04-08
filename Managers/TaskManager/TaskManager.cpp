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
    for(int i = 0; i < topology.getHosts(); ++i) {
        if(topology.isFreeHost(i)) {
            tasks[indTask++].setHostName(topology.getHost(i));
            topology.removeFreeHost(i);
        }
    }
    if (indTask != job.processes) {
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

    for (; countMessage >= 0; --countMessage) {
        ///Отправляем сообщение от мастер ноды
        for (int indTask = 1; indTask < job->processes; ++indTask) {
            tasks[0].addAction({idAction++, ActionType::PUT, tasks[indTask].getHostName(), job->messages[0][indTask].cost});
            tasks[indTask].addAction({idAction++, ActionType::GET, tasks[0].getHostName(), job->messages[indTask][0].cost});
        }

        ///Мастер нода получает сообщения
        for (int indTask = 1; indTask < job->processes; ++indTask) {
            tasks[indTask].addAction({idAction++, ActionType::PUT, tasks[0].getHostName(), job->messages[indTask][0].cost});
            tasks[0].addAction({idAction++, ActionType::GET, tasks[indTask].getHostName(), job->messages[0][indTask].cost});
        }
    }
}

void TaskManager::fillActionsForGrid(std::vector<Task> &tasks) {

}

void TaskManager::fillActionsForCube(std::vector<Task> &tasks) {

}

void TaskManager::fillActionsForTree(std::vector<Task> &tasks) {

}

void TaskManager::addTasks(std::vector<Task> &tasks) {
    for (auto& task : tasks) {
        data.emplace(task.getId(), std::move(task));
    }
}
