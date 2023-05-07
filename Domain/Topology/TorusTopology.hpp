//
// Created by user on 05.04.23.
//

#ifndef DIPLOMA_TORUSTOPOLOGY_H
#define DIPLOMA_TORUSTOPOLOGY_H

#include <vector>
#include <string>
#include <math.h>

#include "Topology.hpp"
#include "../Job.hpp"

class TorusTopology : public Topology {
    int sizeX, sizeY, sizeZ;
    std::vector<int> hilbertCurve;

    void buildHilbertCurve();

    void calcHilbertCurve(int x, int y, int z,
                          int ax, int ay, int az,
                          int bx, int by, int bz,
                          int cx, int cy, int cz); // Очень страшно
public:
    TorusTopology(std::string_view prefix, std::string_view suffix, int sizeX, int sizeY, int sizeZ);

    int getHostNumber(int x, int y, int z);
    const std::vector<int>& getHilbertCurve() const;
    bool isFreeHostByNumHilbert(int i);
    std::string_view getHostByNumHilbert(int i);
    int getHostNumByNumHilbert(int i);
};


#endif //DIPLOMA_TORUSTOPOLOGY_H
