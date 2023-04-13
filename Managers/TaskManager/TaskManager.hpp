#ifndef DIPLOMA_TASKMANAGER_HPP
#define DIPLOMA_TASKMANAGER_HPP

#include <unordered_map>

#include "../../Domain/Topology/Topology.hpp"
#include "../SimGridManager.hpp"
#include "../../Domain/Task.hpp"
#include "../../Utils/Point2dUtils.hpp"
#include "../../Utils/Point3dUtils.hpp"

class SimGridManager;

class TaskManager {

    inline static std::unordered_map<int, Task> data;

protected:
    TaskManager();

    using PlacementMode = Job::PlacementMode;
    using JobType = Job::JobType;
    using ActionType = Action::ActionType;
    SimGridManager& simGridManager;

    inline static int idTask;
    inline static int idAction;

    void createTaskInSimplePlacementMode(Job job, Topology& topology);
    virtual void createTaskInOptimalPlacementMode(Job job, Topology& topology) = 0;
    virtual void createTaskInAdvancedPlacementMode(Job job, Topology& topology) = 0;
    void createTaskInRandomPlacementMode(Job job, Topology& topology);

    void fillActions(std::vector<Task>& tasks);
    void fillActionsForStar(std::vector<Task>& tasks);
    void fillActionsForGrid(std::vector<Task>& tasks);
    void fillActionsForCube(std::vector<Task>& tasks);
    void fillActionsForTree(std::vector<Task>& tasks);
    void fillActionsForFull(std::vector<Task>& tasks);

    static void addAction(std::vector<Task>& tasks,int taskSender, int taskReciever, unsigned long cost);

    void addTasks(std::vector<Task>& tasks);
public:
    static const std::unordered_map<int, Task>& getAll();

    virtual void createTasks(const Job& job) = 0;

    static void updateTimeTask(const Task& task);
};


#endif //DIPLOMA_TASKMANAGER_HPP
