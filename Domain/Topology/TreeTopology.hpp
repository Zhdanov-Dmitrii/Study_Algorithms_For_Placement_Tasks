#ifndef DIPLOMA_TREETOPOLOGY_HPP
#define DIPLOMA_TREETOPOLOGY_HPP

#include "Topology.hpp"

#include <vector>
#include <set>
#include <algorithm>
#include <memory>
#include <stdexcept>

class TreeTopology : public Topology{
public:
    struct Switch {
        std::vector<std::shared_ptr<Switch>> down;
        std::vector<std::weak_ptr<Switch>> up;
        int nodes;
        std::set<int> freeNodes;
    };

    TreeTopology(std::string_view prefix, std::string_view suffix, int nodes, int lvls, int down, int up);

    void removeFreeHost(int host) override;
    int getCountLevels() const;
    std::vector<std::shared_ptr<Switch>>& getLevel(int lvl);
private:
    std::vector<std::vector<std::shared_ptr<Switch>>> levels;

    void removeFreeHost(std::weak_ptr<Switch>& sw, int host);
};


#endif //DIPLOMA_TREETOPOLOGY_HPP
