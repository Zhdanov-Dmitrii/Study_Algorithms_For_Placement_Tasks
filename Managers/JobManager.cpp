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
    int n = cbrt(job.processes);

    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            for (int z = 0; z < n; ++z) {
                // этот зверь принимает функцию получения соседа и устанавливает сообщение для него
                auto setMsg = [&messages, n, x, y, z, countP2pMessages, cost]
                        (Point3D (*func)(const Point3D &, const Point3D &, const Point3D &)) {
                    auto getNum = [n](const Point3D& p) {return p.x + p.y * n + p.z * n * n;};
                    Point3D neighbor = func({x, y}, {n-1,n-1,n-1}, {0,0,0});
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

}


