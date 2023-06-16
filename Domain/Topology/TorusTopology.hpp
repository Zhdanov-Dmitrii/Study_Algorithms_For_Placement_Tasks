//
// Created by user on 05.04.23.
//

#ifndef DIPLOMA_TORUSTOPOLOGY_H
#define DIPLOMA_TORUSTOPOLOGY_H

#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <algorithm>

#include "MeshTopology.hpp"
#include "../Job.hpp"
#include "../NeighborTree.hpp"
#include "../../Utils/Metis/CSR.hpp"
#include "../../Utils/Point3dUtils.hpp"
#include "../../Utils/DisjointSet.hpp"

class TorusTopology : public MeshTopology {
    int sizeX, sizeY, sizeZ;

    void buildHilbertCurve();

    void calcHilbertCurve(int x, int y, int z,
                          int ax, int ay, int az,
                          int bx, int by, int bz,
                          int cx, int cy, int cz); // Очень страшно

//    static Neighbor createNeighborTree(const std::vector<int>& indHosts, CSR& csr, const std::vector<int>& map);
//    static Neighbor createNeighbor(const std::vector<int>& indHosts, int l, int r);
//    static NeighborTree createNeighborKruskal(std::vector<int>& indHosts, const TorusTopology& topology);
public:
    TorusTopology(std::string_view prefix, std::string_view suffix, int sizeX, int sizeY, int sizeZ);

    int getHostNumber(int x, int y, int z);
    Point3D getPointByNum(int i) const;

//    static NeighborTree createNeighborTree(std::vector<int>& indHosts, const TorusTopology& topology);
};


#endif //DIPLOMA_TORUSTOPOLOGY_H
