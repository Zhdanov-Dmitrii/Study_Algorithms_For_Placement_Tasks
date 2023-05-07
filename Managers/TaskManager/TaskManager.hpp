#ifndef DIPLOMA_TASKMANAGER_HPP
#define DIPLOMA_TASKMANAGER_HPP

#include <unordered_map>

#include "../../Domain/Topology/Topology.hpp"
#include "../../Domain/Task.hpp"
#include "../../Utils/Point2dUtils.hpp"
#include "../../Utils/Point3dUtils.hpp"

class TaskManager {
protected:
    TaskManager();

    using PlacementMode = Job::PlacementMode;
    using JobType = Job::JobType;
    using ActionType = Action::ActionType;

    inline static unsigned long long idTask;
    inline static unsigned long long idAction;

    inline static std::map<unsigned long long, Task> tasks;
    inline static std::unordered_map<unsigned long long, std::shared_ptr<Job>> jobs; //todo разные айди для

    void createTaskInSimplePlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology);
    virtual void createTaskInOptimalPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) = 0;
    virtual void createTaskInAdvancedPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology) = 0;
    void createTaskInRandomPlacementMode(std::shared_ptr<Job>& jobPtr, Topology& topology);

    void fillActions(std::vector<Task>& tasks);
    void fillActionsForStar(std::vector<Task>& tasks);
    void fillActionsForGrid(std::vector<Task>& tasks);
    void fillActionsForCube(std::vector<Task>& tasks);
    void fillActionsForTree(std::vector<Task>& tasks);
    void fillActionsForFull(std::vector<Task>& tasks);

    static void addAction(std::vector<Task>& tasks,int taskSender, int taskReciever, unsigned long cost);

    static void fillTree(std::vector<Task>& tasks, int x, bool flag);

    void addTasks(std::vector<Task>& tasks);
public:
    static const std::map<unsigned long long int, Task> & getAllTasks();

    static const std::unordered_map<unsigned long long int, std::shared_ptr<Job>> & getAllJobs();

    virtual void createTasks(Job job) = 0;

    static void updateTimeTask(const Task& task);

    static void calcJobTime();

    virtual void clear() = 0;

    static void removeInvalidJobs();
};


#endif //DIPLOMA_TASKMANAGER_HPP
