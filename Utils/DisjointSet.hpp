#ifndef DIPLOMA_DISJOINTSET_HPP
#define DIPLOMA_DISJOINTSET_HPP

#include <vector>

class DisjointSet {
    std::vector<int> rank;
    std::vector<int> parent;

public:
    DisjointSet(int n);

    int find(int a);
    void union_set(int a, int b);
};


#endif //DIPLOMA_DISJOINTSET_HPP
