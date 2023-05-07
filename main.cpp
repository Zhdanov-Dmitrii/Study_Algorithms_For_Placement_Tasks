#include <simgrid/s4u.hpp>

#include <iostream>
#include "Managers/DeployManager.hpp"
#include "Domain/Topology/TreeTopology.hpp"

using namespace simgrid::s4u;

int main() {
    srand(time(NULL));


    DeployManager& deployManager = DeployManager::getInstance();
    deployManager.setDeployFilePath("deploy.txt");
    deployManager.run();

    return 0;
}