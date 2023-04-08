#include <simgrid/s4u.hpp>

#include <iostream>
#include "Managers/JobManager.hpp"

using namespace simgrid::s4u;

int main() {
    JobManager& jobManager = JobManager::getInstance();
    jobManager.createJob(Job::JobType::STAR, Job::PlacementMode::RANDOM, 512, 5, 1000);
    jobManager.run();

    return 0;
}