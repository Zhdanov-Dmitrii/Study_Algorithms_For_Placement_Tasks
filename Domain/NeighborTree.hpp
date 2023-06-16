#ifndef DIPLOMA_NEIGHBORTREE_HPP
#define DIPLOMA_NEIGHBORTREE_HPP

#include <vector>
#include <memory>

class NeighborTree {
public:
    struct Neighbor {
        int hostCount;
        bool isLeaf;
        std::vector<int> hosts;
        std::vector<std::shared_ptr<Neighbor>> neighbors;
    };
    std::shared_ptr<Neighbor> root;
};


#endif //DIPLOMA_NEIGHBORTREE_HPP
