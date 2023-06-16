#include "CSR.hpp"

std::vector<idx_t> &CSR::partition(std::vector<int>& sizeP) {
    nparts = sizeP.size();
    parts.resize(nvtx);
    tpwgt.clear();
    for (auto p : sizeP) {
        tpwgt.push_back((real_t)p/(real_t)nvtx);
    }

    METIS_PartGraphKway(&nvtx, &ncon, xadj.data(), adjncy.data(),
                        nullptr, nullptr, nullptr, &nparts,
                        tpwgt.data(), nullptr, nullptr, &objval, parts.data());
    std::vector<int> checkSizeP(nparts);
    for (int i = 0; i < nvtx; ++i) {
        ++checkSizeP[parts[i]];
    }

    if (std::equal(sizeP.begin(), sizeP.end(), checkSizeP.begin(), checkSizeP.end())) {
        return parts;
    }

    /// балансировка
    /// можно сделать быстрее и, наверное, оптимальнее
    for (int i = 0; i < sizeP.size(); ++i) {
        while (checkSizeP[i] < sizeP[i]) {
            int maxE = -1;
            int resV = -1;
            for (int j = 0; j < nvtx; ++j) {
                if (parts[j] == i || checkSizeP[parts[j]] <= sizeP[parts[j]]) continue;

                int countE = 0;
                for (int k = xadj[j]; k < xadj[j+1]; ++k)
                    if (parts[adjncy[k]] == i)
                        ++countE;


                if (countE >= maxE) {
                    maxE = countE;
                    resV = j;
                }
            }

            ++checkSizeP[i];
            --checkSizeP[parts[resV]];

            parts[resV] = i;
        }


//        if (checkSizeP[i] >= sizeP[i]) continue;
//
//        for (int j = 0; j < nvtx && checkSizeP[i] < sizeP[i]; ++j) {
//            if (checkSizeP[parts[j]] > sizeP[parts[j]]) {
//                ++checkSizeP[i];
//                --checkSizeP[parts[j]];
//
//                parts[j] = i;
//            }
//        }
    }

    if (!std::equal(sizeP.begin(), sizeP.end(), checkSizeP.begin(), checkSizeP.end())) {
        throw std::runtime_error("sizeP != checkSizeP");
    }

    return parts;
}

std::vector<idx_t> &CSR::bipatition() {
    nparts = 2;
    parts.resize(nvtx);
    METIS_PartGraphKway(&nvtx, &ncon, xadj.data(), adjncy.data(),
                        nullptr, nullptr, nullptr, &nparts,
                        nullptr, nullptr, nullptr, &objval, parts.data());

    std::unordered_map<int,int> sizeP;
    for (int i = 0; i < nvtx; ++i) {
        ++sizeP[parts[i]];
    }

    /// иногда разбивает графы на не равные части
    if (sizeP.size() == 2) {
        if (nvtx % 2 == abs(sizeP[0] - sizeP[1]))
            return parts;
    }

    /// Балансировка
    /// Очень сомнительное решение, но не знаю как лучше
    int i=0;
    while (nvtx % 2 != abs(sizeP[0] - sizeP[1])){
        if (sizeP[0] > sizeP[1]) {
            while (parts[i] != 0) ++i;
            parts[i] = 1;

            --sizeP[0];
            ++sizeP[1];
        } else {
            while (parts[i] != 1) ++i;
            parts[i] = 0;

            ++sizeP[0];
            --sizeP[1];
        }
    }
}

CSR CSR::getCSRFromPart(idx_t p) {
    CSR res;
    res.nvtx = 0;
    res.xadj.push_back(0);

    std::map<idx_t, idx_t> indMap;
    int x = 0;
    for (int i = 0; i < nvtx; ++i) {
        if (parts[i] == p) {
            indMap[i] = x++;
            ++res.nvtx;
        }
    }

    x = 0;
    for (auto& [lastI, newI] : indMap) {
        for (int i = xadj[lastI]; i < xadj[lastI + 1]; ++i) {
            if (parts[adjncy[i]] == p) {
                res.adjncy.push_back(indMap[adjncy[i]]);
                ++x;

                if (!adjwgt.empty()) {
                    res.adjwgt.push_back(adjwgt[i]);
                }
            }
        }
        res.xadj.push_back(x);
    }

    return res;
}

