#include "TorusTopology.hpp"

TorusTopology::TorusTopology(const std::string_view prefix, const std::string_view suffix, int sizeX, int sizeY, int sizeZ)
        : Topology(prefix, suffix, sizeX * sizeY * sizeZ), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {

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

const std::vector<int> &TorusTopology::getHilbertCurve() const {
    return hilbertCurve;
}

bool TorusTopology::isFreeHostByNumHilbert(int i) {
    return isFreeHost(hilbertCurve[i]);
}

std::string_view TorusTopology::getHostByNumHilbert(int i) {
    return getHost(hilbertCurve[i]);
}

int TorusTopology::getHostNumByNumHilbert(int i) {
    return hilbertCurve[i];
}
