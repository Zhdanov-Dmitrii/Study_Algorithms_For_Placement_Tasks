#include "JobManager.hpp"

JobManager::JobManager()
    : torusTaskManager(TorusTaskManager::getInstance()),
      simGridManager(SimGridManager::getInstance()) {

}

JobManager& JobManager::getInstance() {
    static JobManager instance;
    return instance;
}

void JobManager::createJob(Job::JobType jobType, PlacementMode placementMode, int processes, int countP2pMessage, unsigned long cost) {
    static int id;

    Job job;
    job.id = id++;
    job.processes = processes;
    job.jobType = jobType;
    job.placementMode = placementMode;

    switch (jobType) {
        case JobType::STAR:
            createStarJob(job, countP2pMessage, cost);
            break;
        case JobType::GRID:
            createGridJob(job, countP2pMessage, cost);
            break;
        case JobType::CUBE:
            createCubeJob(job, countP2pMessage, cost);
            break;
        case JobType::TREE:
            createTreeJob(job, countP2pMessage, cost);
            break;
        default: createFullJob(job, countP2pMessage, cost);
    }

    jobs.emplace(job.id ,std::move(job));
}

void JobManager::run() {
    for (auto &[id, job] : jobs) {
        torusTaskManager.createTasks(job);
    }

    simGridManager.registerTasks();
    simGridManager.run();
}

void JobManager::createFullJob(Job &job, int countP2pMessage, unsigned long cost) {
    auto& messages = job.messages;
    messages.resize(job.processes);
    for (int i = 0; i < job.processes; ++i) {
        messages[i].reserve(job.processes-1);
        for(int j = 0; j < job.processes; ++j) {
            if (j == i) continue;
            messages[i].push_back({j, countP2pMessage, cost});
        }
    }
}

void JobManager::createStarJob(Job &job, int countP2pMessage, unsigned long cost) {
    auto& messages = job.messages;
    messages.resize(job.processes);
    messages[0].reserve(job.processes-1);

    for (int i = 1; i < job.processes; ++i) {
        messages[0].push_back({i ,countP2pMessage, cost});
        messages[i].push_back({0, countP2pMessage, cost});
    }
}

void JobManager::createGridJob(Job &job, int countP2pMessage, unsigned long cost) {
}

void JobManager::createCubeJob(Job &job, int countP2pMessage, unsigned long cost) {
}

void JobManager::createTreeJob(Job &job, int countP2pMessage, unsigned long cost) {
}


