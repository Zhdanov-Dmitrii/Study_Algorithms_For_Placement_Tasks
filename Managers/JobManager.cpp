#include <iostream>
#include "JobManager.hpp"

JobManager::JobManager()
    : torusTaskManager(TorusTaskManager::getInstance()),
      treeTaskManager(TreeTaskManager::getInstance()),
      simGridManager(SimGridManager::getInstance()),
      topologyManager(TopologyManager::getInstance()) {}

JobManager& JobManager::getInstance() {
    static JobManager instance;
    return instance;
}

void JobManager::run(TopologyType topologyType) {

    std::shared_ptr<Topology> topology;
    std::shared_ptr<TaskManager> taskManager;
    switch (topologyType) {
        case TopologyManager::TORUS_TOPOLOGY:
            topology = std::make_shared<TorusTopology>(topologyManager.getTorusTopology());
            break;
        case TopologyType::GRID_TOPOLOGY:
            topology = std::make_shared<GridTopology>(topologyManager.getGridTopology());

            break;
        case TopologyType::FAT_TREE_TOPOLOGY:
            topology = std::make_shared<TreeTopology>(topologyManager.getFatTreeTopology());
            break;
        case TopologyType::THIN_TREE_TOPOLOGY:
            topology = std::make_shared<TreeTopology>(topologyManager.getThinTreeTopology());
            break;
    }

    if (topologyType == TopologyType::TORUS_TOPOLOGY || topologyType == TopologyType::GRID_TOPOLOGY) {
        for (auto &[id, job] : jobs) {
            torusTaskManager.createTasks(job, *topology);
        }
    }else if (topologyType == TopologyType::FAT_TREE_TOPOLOGY || topologyType == TopologyType::THIN_TREE_TOPOLOGY) {
        for (auto &[id, job] : jobs) {
            treeTaskManager.createTasks(job, *topology);
        }
    }

    simGridManager.registerTasks();
    simGridManager.run();
    TaskManager::calcJobTime();
}

void JobManager::createJob(JobType jobType, PlacementMode placementMode, bool isValid, int processes,
                           int countP2pMessage, unsigned long cost, std::string_view path) {
    if (processes == 0)
        return;

    Job job;
    job.id = idJob++;
    job.isValid = isValid;
    job.processes = processes;
    job.jobType = jobType;
    job.placementMode = placementMode;
    job.path = path;

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
    auto& messages = job.messages;
    messages.resize(job.processes);
    int n = sqrt(job.processes);

    //по y с четными
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            // этот зверь принимает функцию получения соседа и устанавливает сообщение для него
            auto setMsg = [&messages, n, x, y, countP2pMessage, cost]
                    (Point2D (*func)(const Point2D &, const Point2D &, const Point2D &)) {
                Point2D neighbor = func({x, y}, {n-1,n-1}, {0,0});
                messages[x + n*y].push_back({neighbor.x + n*neighbor.y, countP2pMessage, cost});
            };

            setMsg(&Point2dUtils::getUpPoint);
            setMsg(&Point2dUtils::getDownPoint);
            setMsg(&Point2dUtils::getLeftPoint);
            setMsg(&Point2dUtils::getRightPoint);
        }
    }
}

void JobManager::createCubeJob(Job &job, int countP2pMessages, unsigned long cost) {
    auto& messages = job.messages;
    messages.resize(job.processes);
    int n = std::round(cbrt(job.processes));

    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            for (int z = 0; z < n; ++z) {
                // этот зверь принимает функцию получения соседа и устанавливает сообщение для него
                auto setMsg = [&messages, n, x, y, z, countP2pMessages, cost]
                        (Point3D (*func)(const Point3D &, const Point3D &, const Point3D &)) {
                    auto getNum = [n](const Point3D& p) {return p.x + p.y * n + p.z * n * n;};
                    Point3D neighbor = func({x, y, z}, {n-1,n-1,n-1}, {0,0,0});
                    messages[getNum({x,y,z})].push_back({getNum(neighbor), countP2pMessages, cost});
                };

                setMsg(&Point3dUtils::getUpPoint);
                setMsg(&Point3dUtils::getDownPoint);
                setMsg(&Point3dUtils::getLeftPoint);
                setMsg(&Point3dUtils::getRightPoint);
                setMsg(&Point3dUtils::getBehindPoint);
                setMsg(&Point3dUtils::getBeforePoint);
            }
        }
    }

}

void JobManager::createTreeJob(Job &job, int countP2pMessage, unsigned long cost) {
    auto& messages = job.messages;
    messages.resize(job.processes);

    fillTree(job, 0, countP2pMessage, cost);
}

void JobManager::clear() {
    idJob = 0;
    jobs.clear();
}

void JobManager::removeInvalidJob() {
    std::vector<int> remId;
    for (auto &[id, job] : jobs) {
        if (!job.isValid) {
            remId.push_back(id);
        }
    }

    for(auto id : remId)
        jobs.erase(id);
}

void JobManager::fillTree(Job &job, int i, int countP2pMessage, unsigned long cost) {
    auto getParent = [](int x) { return x / 2 - 1 + x % 2; };
    auto getLeftChild = [](int x) { return x * 2 + 1; };
    auto getRightChild = [](int x) { return x * 2 + 2; };

    int lChild = getLeftChild(i);
    int rChild = getRightChild(i);

    if (i != 0) {
        job.messages[i].push_back({getParent(i), countP2pMessage, cost});
    }

    if (lChild < job.processes) {
        job.messages[i].push_back({lChild, countP2pMessage, cost});
        fillTree(job, lChild, countP2pMessage, cost);
    }

    if (rChild < job.processes) {
        job.messages[i].push_back({rChild, countP2pMessage, cost});
        fillTree(job, rChild, countP2pMessage, cost);
    }

}

std::map<int, Job>& JobManager::getJobs() {
    return jobs;
}


