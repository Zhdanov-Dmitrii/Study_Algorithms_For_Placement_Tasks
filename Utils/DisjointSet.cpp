#include "DisjointSet.hpp"

DisjointSet::DisjointSet(int n) {
    rank.resize(n);
    parent.reserve(n);
    for (int i = 0; i < n; ++i) {
        parent.push_back(i);
    }
}

int DisjointSet::find(int a) {
    if (parent[a] != a) {
        parent[a] = find(parent[a]);
    }
    return parent[a];
}

void DisjointSet::union_set(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) {
        if (rank[a] < rank[b])
            std::swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b])
            rank[a]++;
    }
}
