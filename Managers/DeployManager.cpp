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
                removeInvalidJob();

            jobManager.createJob(jobType, placementMode, isValid, processes, countP2pMessage, cost, resPath);
        }

        TopologyManager::TopologyType topologyType = session["topologyType"];
        jobManager.run(topologyType);

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
    jobManager.clear();
    simGridManager.killAllActor();
}

void DeployManager::removeInvalidJob() {
    jobManager.removeInvalidJob();
    TaskManager::removeInvalidJobs();
}

