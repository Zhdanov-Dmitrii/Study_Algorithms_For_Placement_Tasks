#include <simgrid/s4u.hpp>

#include <iostream>
#include "Managers/DeployManager.hpp"

using namespace simgrid::s4u;

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc < 2) {
        std::cout << "no deploy file" << std::endl;
        return 0;
    }

    DeployManager& deployManager = DeployManager::getInstance();
    deployManager.setDeployFilePath(argv[1]);
    deployManager.run();

    std::cout << "finish" << std::endl;

    return 0;
}