#ifndef DIPLOMA_CSR_HPP
#define DIPLOMA_CSR_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <map>
#include <math.h>
#include <metis.h>
#include <algorithm>

struct CSR {
    idx_t nvtx;
    idx_t ncon = 1;
    std::vector<idx_t> xadj;
    std::vector<idx_t> adjncy;
    std::vector<idx_t> adjwgt;
    idx_t nparts;
    std::vector<real_t> tpwgt;
    std::vector<idx_t> parts;
    idx_t objval;

    std::vector<idx_t>& partition(std::vector<int>& sizeP);
    std::vector<idx_t>& bipatition();

    CSR getCSRFromPart(idx_t p);
};


#endif //DIPLOMA_CSR_HPP
