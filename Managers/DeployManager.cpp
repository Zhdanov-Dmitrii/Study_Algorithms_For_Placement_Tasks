#include <iostream>
#include "DeployManager.hpp"

DeployManager::DeployManager() : jobManager(JobManager::getInstance()),
    simGridManager(SimGridManager::getInstance()),
    torusTaskManager(TorusTaskManager::getInstance()),
    treeTaskManager(TreeTaskManager::getInstance()){

}

DeployManager &DeployManager::getInstance() {
    static DeployManager instance;
    return instance;
}

void DeployManager::setDeployFilePath(std::string_view path) {
    this->path = path;
}

void DeployManager::run() {
    assert(!path.empty());

    std::ifstream ifs(path);
    Json json = Json::parse(ifs);

    clock_t start = clock();
    for (auto &session : json) {
        for (auto &job : session["jobs"]) {
            JobType jobType = job["jobType"];
            PlacementMode placementMode = job["placementMode"];
            bool isValid = job["isValid"];
            int processes = job["processes"];
            int countP2pMessage = job["countP2pMessage"];
            unsigned long cost = job["cost"];
            std::string resPath = job["path"];

            if (!resPath.empty())
                std::cout << resPath << " " << cost << " ";

            if (!resPath.empty())
                removeInvalidJob();

            jobManager.createJob(jobType, placementMode, isValid, processes, countP2pMessage, cost, resPath);
        }
        double timePlace = (double)(clock()-start)/CLOCKS_PER_SEC;

        JobManager::TopologyType topologyType = session["topologyType"];
        start = clock();
        jobManager.run(topologyType);
        double timeRun = (double)(clock()-start)/CLOCKS_PER_SEC;
        std::cout << "; TP = " << timePlace << "; TR = " << timeRun << std::endl;

        saveResult();
        clearState();
    }
}

void DeployManager::saveResult() {
    for (auto& [id, job] : TaskManager::getAllJobs()) {
        if (job->path.empty()) continue;

        std::ofstream fout(job->path, std::ios::app);
        fout << job->time << " ";
    }
}

void DeployManager::clearState() {
    torusTaskManager.clear();
    treeTaskManager.clear();
    jobManager.clear();
    simGridManager.killAllActor();
}

void DeployManager::removeInvalidJob() {
    jobManager.removeInvalidJob();
    TaskManager::removeInvalidJobs();
}

