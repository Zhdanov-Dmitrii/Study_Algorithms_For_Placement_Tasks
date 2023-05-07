//
// Created by user on 18.04.23.
//

#include "TreeTopology.hpp"
#include <iostream>

TreeTopology::TreeTopology(std::string_view prefix, std::string_view suffix, int nodes, int lvls, int down, int up)
        : Topology(prefix, suffix, nodes) {
    levels.resize(lvls);
    int currSwitches = nodes / down;
    int nodesInSwitch = down;

    int step = 1;

    //заполняем нижний уровень
    for (int sw = 0; sw < currSwitches; ++sw) {
        std::set<int> t;
        for (int indHost = 0; indHost < nodesInSwitch; ++indHost)
            t.insert(sw*down + indHost);

        Switch aSwitch = {{},{}, nodesInSwitch, std::move(t)};
        levels[0].push_back(std::make_shared<Switch>(aSwitch));
    }

    for (int lvl = 1; lvl < lvls; ++lvl) {
        currSwitches /= down/up;
        nodesInSwitch *= up;

        int start = -1;
        for (int sw = 0; sw < currSwitches; ++sw) {
            Switch aSwitch = {{},{}, nodesInSwitch,{}};
            levels[lvl].push_back(std::make_shared<Switch>(aSwitch));

            std::set<int> t;//свободные узлы


            int shift = (sw%(step*up))/step;
            start = sw % (step*up) == 0 ? sw * down/up : start + 1;
            for (int i = 0 ; i < down; ++i) {
                int ind = start + step * (i-shift);
                levels[lvl].back()->down.push_back(levels[lvl-1][ind]);
                levels[lvl-1][ind]->up.push_back(levels[lvl].back());

                for(auto host : levels[lvl-1][ind]->freeNodes)
                    t.insert(host);
            }

            levels[lvl].back()->freeNodes = std::move(t);
        }

        step *= up;
    }
    int tmp = levels.back().back()->freeNodes.size();
    tmp++;
}

void TreeTopology::removeFreeHost(int host) {
    int down = hosts / levels[0].size();
    if (!levels[0][host/down]->freeNodes.count(host)){
        throw std::invalid_argument(std::to_string(host) + " is no free host");
    }

    std::weak_ptr<Switch> downSw = levels[0][host/down];
    removeFreeHost(downSw, host);
    freeHosts.erase(host);
}

void TreeTopology::removeFreeHost(std::weak_ptr<Switch>& sw, int host) {
    sw.lock()->freeNodes.erase(host);
    for (auto& nextSw : sw.lock()->up) {
        removeFreeHost(nextSw, host);
    }
}

std::vector<std::shared_ptr<TreeTopology::Switch>>& TreeTopology::getLevel(int lvl) {
    return levels[lvl];
}

int TreeTopology::getCountLevels() const {
    return levels.size();
}
