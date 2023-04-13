#ifndef DIPLOMA_POINT3DUTILS_HPP
#define DIPLOMA_POINT3DUTILS_HPP

#include "Points/Point3D.hpp"

struct Point3dUtils {
    static Point3D getUpPoint (const Point3D& point, const Point3D& rPoint, const Point3D& lPoint = {0,0,0});
    static Point3D getDownPoint (const Point3D& point, const Point3D& rPoint, const Point3D& lPoint = {0,0,0});
    static Point3D getLeftPoint (const Point3D& point, const Point3D& rPoint, const Point3D& lPoint = {0,0,0});
    static Point3D getRightPoint (const Point3D& point, const Point3D& rPoint, const Point3D& lPoint = {0,0,0});
    static Point3D getBehindPoint (const Point3D& point, const Point3D& rPoint, const Point3D& lPoint = {0,0,0});
    static Point3D getBeforePoint (const Point3D& point, const Point3D& rPoint, const Point3D& lPoint = {0,0,0});
};


#endif //DIPLOMA_POINT3DUTILS_HPP
