#ifndef DIPLOMA_GRIDTOPOLOGY_HPP
#define DIPLOMA_GRIDTOPOLOGY_HPP

#include "MeshTopology.hpp"

class GridTopology : public MeshTopology {
    int sizeX, sizeY;

public:
    GridTopology(std::string_view prefix, std::string_view suffix, int sizeX, int sizeY);
};


#endif //DIPLOMA_GRIDTOPOLOGY_HPP
