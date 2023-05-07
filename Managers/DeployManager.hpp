//
// Created by user on 14.04.23.
//

#ifndef DIPLOMA_DEPLOYMANAGER_HPP
#define DIPLOMA_DEPLOYMANAGER_HPP

#include <string>
#include <fstream>

#include "JobManager.hpp"
#include "TaskManager/TaskManager.hpp"
#include "../Utils/json.hpp"

class DeployManager {
    using Json = nlohmann::json;
    using JobType = Job::JobType;
    using PlacementMode = Job::PlacementMode;

    DeployManager();
    DeployManager(const DeployManager&);
    DeployManager& operator=(DeployManager&);

    JobManager& jobManager;
    TorusTaskManager& torusTaskManager;
    TreeTaskManager& treeTaskManager;
    SimGridManager& simGridManager;

    std::string path;

    void clearState();
    void saveResult();
    void removeInvalidJob();

public:
    static DeployManager& getInstance();
    void setDeployFilePath(std::string_view path);

    void run();
};


#endif //DIPLOMA_DEPLOYMANAGER_HPP
