#include "TorusTopology.hpp"

TorusTopology::TorusTopology(const std::string_view prefix, const std::string_view suffix, int sizeX, int sizeY, int sizeZ)
        : MeshTopology(prefix, suffix, sizeX * sizeY * sizeZ), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {

    hilbertCurve.reserve(sizeX * sizeY * sizeZ);
    buildHilbertCurve();
}

void TorusTopology::buildHilbertCurve() {
    calcHilbertCurve(0,0,0,
                     sizeX, 0, 0,
                     0, sizeY, 0,
                     0, 0, sizeZ);
}

int TorusTopology::getHostNumber(int x, int y, int z) {
    return x + y * sizeX + z * sizeX * sizeY;
}

int sign(int x) {
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return 0;
}

void TorusTopology::calcHilbertCurve(int x, int y, int z,
                                     int ax, int ay, int az,
                                     int bx, int by, int bz,
                                     int cx, int cy, int cz) {

    int w = abs(ax + ay + az);
    int h = abs(bx + by + bz);
    int d = abs(cx + cy + cz);

    int dax = sign(ax), day = sign(ay), daz = sign(az);
    int dbx = sign(bx), dby = sign(by), dbz = sign(bz);
    int dcx = sign(cx), dcy = sign(cy), dcz = sign(cz);

    if (h == 1 && d == 1) {
        for (int i = 0; i < w; ++i) {
            hilbertCurve.push_back(getHostNumber(x, y, z));
            x += dax;
            y += day;
            z += daz;
        }
        return;
    }

    if (w == 1 && d == 1) {
        for (int i = 0; i < h; ++i) {
            hilbertCurve.push_back(getHostNumber(x, y, z));
            x += dbx;
            y += dby;
            z += dbz;
        }
        return;
    }

    if (w == 1 && h == 1) {
        for (int i = 0; i < d; ++i) {
            hilbertCurve.push_back(getHostNumber(x, y, z));
            x += dcx;
            y += dcy;
            z += dcz;
        }
        return;
    }


    int ax2 = ax / 2, ay2 = ay / 2, az2 = az / 2;
    int bx2 = bx / 2, by2 = by / 2, bz2 = bz / 2;
    int cx2 = cx / 2, cy2 = cy / 2, cz2 = cz / 2;

    int w2 = abs(ax2 + ay2 + az2);
    int h2 = abs(bx2 + by2 + bz2);
    int d2 = abs(cx2 + cy2 + cz2);

    if (w2 % 2 && w > 2) {
        ax2 += dax;
        ay2 += day;
        az2 += daz;
    }

    if (w2 % 2 && w > 2) {
        ax2 += dax;
        ay2 += day;
        az2 += daz;
    }

    if (h2 % 2 && h > 2) {
        bx2 += dbx;
        by2 += dby;
        bz2 += dbz;
    }

    if (d2 % 2 && d > 2) {
        cx2 += dcx;
        cy2 += dcy;
        cz2 += dcz;
    }

    if (2 * w > 2 * h && 2 * w > 3 * d) {
        calcHilbertCurve(x, y, z,
                         ax2, ay2, az2,
                         bx, by, bz,
                         cx, cy, cz);

        calcHilbertCurve(x + ax2, y + ay2, z + az2,
                         ax - ax2, ay - ay2, az - az2,
                         bx, by, bz,
                         cx, cy, cz);

    } else if (3 * h > 4 * d) {
        calcHilbertCurve(x, y, z,
                         bx2, by2, bz2,
                         cx, cy, cz,
                         ax2, ay2, az2);

        calcHilbertCurve(x + bx2, y + by2, z + bz2,
                         ax, ay, az,
                         bx - bx2, by - by2, bz - bz2,
                         cx, cy, cz);

        calcHilbertCurve(x + (ax - dax) + (bx2 - dbx),
                         y + (ay - day) + (by2 - dby),
                         z + (az - daz) + (bz2 - dbz),
                         -bx2, -by2, -bz2,
                         cx, cy, cz,
                         -(ax - ax2), -(ay - ay2), -(az - az2));

    } else if (3 * d > 4 * h) {
        calcHilbertCurve(x, y, z,
                         cx2, cy2, cz2,
                         ax2, ay2, az2,
                         bx, by, bz);

        calcHilbertCurve(x + cx2, y + cy2, z + cz2,
                         ax, ay, az,
                         bx, by, bz,
                         cx - cx2, cy - cy2, cz - cz2);

        calcHilbertCurve(x + (ax - dax) + (cx2 - dcx),
                         y + (ay - day) + (cy2 - dcy),
                         z + (az - daz) + (cz2 - dcz),
                         -cx2, -cy2, -cz2,
                         -(ax - ax2), -(ay - ay2), -(az - az2),
                         bx, by, bz);

    } else {
        calcHilbertCurve(x, y, z,
                         bx2, by2, bz2,
                         cx2, cy2, cz2,
                         ax2, ay2, az2);

        calcHilbertCurve(x + bx2, y + by2, z + bz2,
                         cx, cy, cz,
                         ax2, ay2, az2,
                         bx - bx2, by - by2, bz - bz2);

        calcHilbertCurve(x + (bx2 - dbx) + (cx - dcx),
                         y + (by2 - dby) + (cy - dcy),
                         z + (bz2 - dbz) + (cz - dcz),
                         ax, ay, az,
                         -bx2, -by2, -bz2,
                         -(cx - cx2), -(cy - cy2), -(cz - cz2));

        calcHilbertCurve(x + (ax - dax) + bx2 + (cx - dcx),
                         y + (ay - day) + by2 + (cy - dcy),
                         z + (az - daz) + bz2 + (cz - dcz),
                         -cx, -cy, -cz,
                         -(ax - ax2), -(ay - ay2), -(az - az2),
                         bx - bx2, by - by2, bz - bz2);

        calcHilbertCurve(x + (ax - dax) + (bx2 - dbx),
                         y + (ay - day) + (by2 - dby),
                         z + (az - daz) + (bz2 - dbz),
                         -bx2, -by2, -bz2,
                         cx2, cy2, cz2,
                         -(ax - ax2), -(ay - ay2), -(az - az2));
    }
}

Point3D TorusTopology::getPointByNum(int i) const{
    Point3D res{};
    res.x = i % sizeX;
    i /= sizeX;
    res.y = i % sizeY;
    i /= sizeY;
    res.z = i;

    return res;
}

//NeighborTree TorusTopology::createNeighborTree(std::vector<int>& indHosts, const TorusTopology& topology) {
////    return createNeighborKruskal(indHosts, topology);
////
////    CSR csr;
////    csr.nvtx = indHosts.size();
////    csr.xadj.reserve(indHosts.size()+1);
////    csr.adjncy.reserve(indHosts.size()*indHosts.size());
////
////    csr.xadj.push_back(0);
////    int x = 0;
////    for (int i = 0; i < indHosts.size(); ++i) {
////        for (int j = 0; j < indHosts.size(); ++j) {
////            if (i == j) continue;
////            Point3D pi = topology.getPointByNum(i);
////            Point3D pj = topology.getPointByNum(j);
////
////            csr.adjncy.push_back(Point3dUtils::getDistance(pi, pj));
////            ++x;
////        }
////        csr.xadj.push_back(x);
////    }
////    csr.bipatition();
////
////    std::vector<int> map;
////    map.reserve(csr.nvtx);
////    for (int i = 0; i < csr.nvtx; ++i)
////        map.push_back(indHosts[i]);
////
////    NeighborTree res;
////    res.root = std::make_shared<Neighbor>(createNeighborTree(indHosts, csr, map));
//
//
//    std::sort(indHosts.begin(), indHosts.end());
//    NeighborTree res;
//    res.root = std::make_shared<Neighbor>(createNeighbor(indHosts, 0, indHosts.size()));
//
//    return res;
//}
//
//TorusTopology::Neighbor TorusTopology::createNeighborTree(const std::vector<int> &indHosts, CSR &csr,
//                                                          const std::vector<int>& map) {
//    Neighbor neighbor;
//    neighbor.hostCount = csr.nvtx;
//
//    ///Выход
//    if (neighbor.hostCount <= 2) {
//        neighbor.isLeaf = true;
//        for (int i = 0; i < neighbor.hostCount; ++i)
//            neighbor.hosts.push_back(map.at(i));
//
//        return neighbor;
//    }
//    neighbor.isLeaf = false;
//
//
//    for (int i = 0; i < 2; ++i){
//        std::vector<int> newMap;
//        for (int j = 0; j < csr.nvtx; ++j) {
//            if (csr.parts[j] == i)
//                newMap.push_back(map[j]);
//        }
//
//
//        CSR newCSR = csr.getCSRFromPart(i);
//        newCSR.bipatition();
//        neighbor.neighbors.push_back(std::make_shared<Neighbor>(createNeighborTree(indHosts, newCSR, newMap)));
//    }
//
//    return neighbor;
//}
//
//TorusTopology::Neighbor TorusTopology::createNeighbor(const std::vector<int> &indHosts, int l, int r) {
//    Neighbor neighbor;
//    neighbor.hostCount = r-l;
//    if (r-l <= 2) {
//        neighbor.isLeaf = true;
//        for (int i = l; i < r; ++i)
//            neighbor.hosts.push_back(indHosts[i]);
//
//        return neighbor;
//    }
//
//    neighbor.isLeaf = false;
//    neighbor.neighbors.push_back(std::make_shared<Neighbor>(createNeighbor(indHosts,l,(l+r)/2)));
//    neighbor.neighbors.push_back(std::make_shared<Neighbor>(createNeighbor(indHosts,(l+r)/2,r)));
//
//    return neighbor;
//}
//
//NeighborTree TorusTopology::createNeighborKruskal(std::vector<int> &indHosts, const TorusTopology &topology) {
//    struct Edge{
//        int a, b;
//        int d;
//    };
//
//    std::vector<std::shared_ptr<Neighbor>> parents;
//    parents.reserve(indHosts.size());
//    for (int i = 0; i < indHosts.size(); ++i){
//        auto t = std::make_shared<Neighbor>(Neighbor());
//        t->hostCount = 1;
//        t->isLeaf = true;
//        t->hosts = {indHosts[i]};
//        parents.push_back(t);
//    }
//
//    std::vector<Edge> edges;
//    edges.reserve(indHosts.size() * indHosts.size());
//    for (int i = 0; i < indHosts.size(); ++i) {
//        for (int j = 0; j < indHosts.size(); ++j) {
//            if (i == j) continue;
//            Point3D pi = topology.getPointByNum(indHosts[i]);
//            Point3D pj = topology.getPointByNum(indHosts[j]);
//            int d = Point3dUtils::getDistance(pi, pj);
//
//            edges.push_back({i, j, d});
//        }
//    }
//
//    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.d < b.d; });
//
//    DisjointSet disjointSet(indHosts.size());
//    for (auto &e : edges) {
//        int indPA = disjointSet.find(e.a);
//        int indPB = disjointSet.find(e.b);
//
//        if (indPA == indPB)
//            continue;
//
//        std::shared_ptr<Neighbor> a = parents[indPA];
//        std::shared_ptr<Neighbor> b = parents[indPB];
//
//        auto node = std::make_shared<Neighbor>(Neighbor());
//        node->isLeaf = false;
//        node->hostCount = a->hostCount + b->hostCount;
//        node->neighbors.push_back(a);
//        node->neighbors.push_back(b);
//
//        parents[e.a] = node;
//        parents[e.b] = node;
//
//        disjointSet.union_set(indPA, indPB);
//        parents[disjointSet.find(indPA)] = node;
//    }
//
//    NeighborTree res;
//    for (auto& p : parents) {
//        if (p->hostCount == indHosts.size()) {
//            res.root = p;
//            break;
//        }
//    }
//
//    if (!res.root) {
//        throw std::runtime_error("failed to build a neighbor tree");
//    }
//
//    return res;
//}


