//
// Created by user on 05.04.23.
//

#include "Topology.hpp"

Topology::Topology(const std::string_view prefix, const std::string_view suffix, int hosts)
        : prefix(prefix), suffix(suffix), hosts(hosts) {

    freeHosts.clear();
    freeHosts.reserve(this->hosts);
    hostNames.reserve(this->hosts);
    for (int i = 0; i < hosts; ++i) {
        hostNames.push_back(this->prefix + std::to_string(i) + this->suffix);
        freeHosts.insert(i);
    }
}

const std::vector<std::string> &Topology::getHostNames() const {
    return hostNames;
}

const std::string &Topology::getHost(int num) const {
    return hostNames[num];
}

int Topology::getHostNumber(std::string_view name) const {
    return std::stoi(std::string(name.begin() + prefix.size(), name.end() - suffix.size()));
}

int Topology::getRandomFreeHost() {

    size_t dist = random() % freeHosts.size();

    auto it = freeHosts.begin();
    std::advance(it, dist);

    return *it;
}

const std::unordered_set<int> &Topology::getFreeHosts() const {
    return freeHosts;
}

void Topology::removeFreeHost(int host) {
    freeHosts.erase(host);
}

bool Topology::isFreeHost(int host) {
    return freeHosts.count(host);
}

int Topology::getHosts() const {
    return hosts;
}
