#include "GridTopology.hpp"

GridTopology::GridTopology(std::string_view prefix, std::string_view suffix, int sizeX, int sizeY)
    : MeshTopology(prefix, suffix, sizeX*sizeY), sizeX(sizeX), sizeY(sizeY)
{
    hilbertCurve.resize(sizeX * sizeY);

    auto xy2d = [](int n, int x, int y) {
        auto rot = [](int n, int *x, int *y, int rx, int ry) {
            if (ry == 0) {
                if (rx == 1) {
                    *x = n-1 - *x;
                    *y = n-1 - *y;
                }

                //Обмениваем x и y местами
                int t  = *x;
                *x = *y;
                *y = t;
            }
        };

        int rx, ry, s, d=0;
        for (s=n/2; s>0; s/=2) {
            rx = (x & s) > 0;
            ry = (y & s) > 0;
            d += s * s * ((3 * rx) ^ ry);
            rot(s, &x, &y, rx, ry);
        }
        return d;
    };

    for (int x = 0; x < sizeX; ++x){
        for (int y = 0; y < sizeY; ++y) {
            int n = sizeX;
            int d = xy2d(n,x,y);

            hilbertCurve[d] = x + n * y;
        }
    }
}
