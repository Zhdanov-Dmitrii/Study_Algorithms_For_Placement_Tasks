cmake_minimum_required(VERSION 3.23)
project(Diploma)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(SimGrid 3.27 REQUIRED) # This template requires SimGrid v3.27 because we use templated get()

include_directories(${SimGrid_INCLUDE_DIR})
include_directories("/usr/local/include")
link_directories("/usr/local/lib")

set(PROJECT_SOURCES
        main.cpp

        Domain/Task.hpp Domain/Task.cpp
        Domain/Job.hpp
        Domain/Action.hpp
        Domain/Topology/Topology.hpp Domain/Topology/Topology.cpp
        Domain/Topology/TorusTopology.hpp Domain/Topology/TorusTopology.cpp
        Domain/Topology/TreeTopology.hpp  Domain/Topology/TreeTopology.cpp

        Managers/JobManager.hpp Managers/JobManager.cpp
        Managers/SimGridManager.hpp Managers/SimGridManager.cpp
        Managers/TaskManager/TorusTaskManager.hpp Managers/TaskManager/TorusTaskManager.cpp
        Managers/TaskManager/TreeTaskManager.hpp Managers/TaskManager/TreeTaskManager.cpp
        Managers/TaskManager/TaskManager.hpp Managers/TaskManager/TaskManager.cpp
        Managers/DeployManager.hpp Managers/DeployManager.cpp
        Managers/TopologyManager.cpp

        Utils/json.hpp
        Utils/Points/Point2D.hpp Utils/Points/Point3D.hpp
        Utils/Point2dUtils.hpp Utils/Point2dUtils.cpp
        Utils/Point3dUtils.hpp Utils/Point3dUtils.cpp
        Utils/Metis/CSR.cpp Utils/Metis/CSR.hpp
        Utils/DisjointSet.cpp Utils/DisjointSet.hpp Domain/Topology/MeshTopology.cpp Domain/Topology/MeshTopology.hpp Domain/Topology/GridTopology.cpp Domain/Topology/GridTopology.hpp)

add_executable(Diploma ${PROJECT_SOURCES})
target_link_libraries(Diploma ${SimGrid_LIBRARY} metis)