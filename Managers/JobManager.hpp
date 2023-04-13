#ifndef DIPLOMA_JOBMANAGER_H
#define DIPLOMA_JOBMANAGER_H


#include <unordered_map>
#include <functional>

#include "../Domain/Job.hpp"
#include "TaskManager/TorusTaskManager.hpp"
#include "../Utils/Point2dUtils.hpp"
#include "../Utils/Point3dUtils.hpp"
#include "../Utils/Points/Point3D.hpp"
#include "../Utils/Points/Point2D.hpp"


class JobManager {
    ///Singleton
    JobManager();
    JobManager(const JobManager &) = delete;
    JobManager &operator=(JobManager &) = delete;

    std::unordered_map<int, Job> jobs;
    TorusTaskManager& torusTaskManager;
    SimGridManager& simGridManager;


    void createFullJob(Job& job, int countP2pMessage, unsigned long cost);
    void createStarJob(Job& job, int countP2pMessage, unsigned long cost);
    void createGridJob(Job &job, int countP2pMessage, unsigned long cost) ;
    void createCubeJob(Job& job, int countP2pMessages, unsigned long cost);
    void createTreeJob(Job& job, int countP2pMessage, unsigned long cost);

    using JobType = Job::JobType;
    using PlacementMode = Job::PlacementMode;
public:
    static JobManager& getInstance();

    void createJob(JobType jobType, PlacementMode placementMode, int processes, int countP2pMessage, unsigned long cost);

    void run();
};


#endif //DIPLOMA_JOBMANAGER_H
