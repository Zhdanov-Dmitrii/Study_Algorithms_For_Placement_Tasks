#ifndef DIPLOMA_JOBMANAGER_H
#define DIPLOMA_JOBMANAGER_H


#include <unordered_map>
#include <functional>
#include <algorithm>

#include "../Domain/Job.hpp"
#include "TaskManager/TorusTaskManager.hpp"
#include "TaskManager/TreeTaskManager.hpp"
#include "TopologyManager.hpp"

#include "SimGridManager.hpp"
#include "../Utils/Point2dUtils.hpp"
#include "../Utils/Point3dUtils.hpp"
#include "../Utils/Points/Point3D.hpp"
#include "../Utils/Points/Point2D.hpp"


class JobManager {
    ///Singleton
    JobManager();
    JobManager(const JobManager &) = delete;
    JobManager &operator=(JobManager &) = delete;

    std::map<int, Job> jobs;
    TorusTaskManager& torusTaskManager;
    TreeTaskManager& treeTaskManager;
    SimGridManager& simGridManager;
    TopologyManager& topologyManager;

    inline static int idJob = 0;

    void createFullJob(Job& job, int countP2pMessage, unsigned long cost);
    void createStarJob(Job& job, int countP2pMessage, unsigned long cost);
    void createGridJob(Job &job, int countP2pMessage, unsigned long cost) ;
    void createCubeJob(Job& job, int countP2pMessages, unsigned long cost);
    void createTreeJob(Job& job, int countP2pMessage, unsigned long cost);

    using JobType = Job::JobType;
    using PlacementMode = Job::PlacementMode;
    using TopologyType = TopologyManager::TopologyType;

    void fillTree(Job& job, int i, int countP2pMessage, unsigned long cost);
public:

    static JobManager& getInstance();

    void createJob(JobType jobType, PlacementMode placementMode, bool isValid, int processes,
                   int countP2pMessage, unsigned long cost, std::string_view path);

    void run(TopologyType topologyType);

    void clear();

    void removeInvalidJob();


};


#endif //DIPLOMA_JOBMANAGER_H
