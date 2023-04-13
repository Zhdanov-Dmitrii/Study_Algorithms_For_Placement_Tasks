#include <simgrid/s4u.hpp>

#include <iostream>
#include "Managers/JobManager.hpp"

using namespace simgrid::s4u;

int main() {
    JobManager& jobManager = JobManager::getInstance();

    //jobManager.createJob(Job::JobType::STAR, Job::PlacementMode::SIMPLE, 512, 5, 1000);

    for(int i = 0; i < 512; i += 8)
        jobManager.createJob(Job::JobType::STAR, Job::PlacementMode::RANDOM, 8, 5, 1);

    //jobManager.createJob(Job::JobType::GRID, Job::PlacementMode::SIMPLE, 484, 50, 1);

//    for (int i = 0; i < 512; i+=16)
//        jobManager.createJob(Job::JobType::GRID, Job::PlacementMode::SIMPLE, 16, 5, 10000);

    jobManager.run();

    return 0;
}