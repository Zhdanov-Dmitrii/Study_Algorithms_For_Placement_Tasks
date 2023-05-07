//
// Created by user on 05.04.23.
//

#ifndef DIPLOMA_TOPOLOGY_H
#define DIPLOMA_TOPOLOGY_H

#include <vector>
#include <unordered_set>
#include <string>
#include <random>

class Topology {
protected:
    std::string prefix;
    std::string suffix;
    int hosts;
    std::vector<std::string> hostNames;
    std::unordered_set<int> freeHosts;

public:
    Topology(const std::string_view prefix, const std::string_view suffix, int hosts);

    const std::vector<std::string> &getHostNames() const;

    const std::string &getHost(int num) const;

    int getHostNumber(std::string_view name) const;

    int getRandomFreeHost();

    const std::unordered_set<int>& getFreeHosts() const;

    virtual void removeFreeHost(int host);

    bool isFreeHost(int host);

    int getHosts() const;


};


#endif //DIPLOMA_TOPOLOGY_H
