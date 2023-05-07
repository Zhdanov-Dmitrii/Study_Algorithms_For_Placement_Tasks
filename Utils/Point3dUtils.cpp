#include "Point3dUtils.hpp"

Point3D Point3dUtils::getUpPoint(const Point3D &point, const Point3D &rPoint, const Point3D &lPoint) {
    Point3D res;
    res.x = point.x;
    res.y = point.y > lPoint.y ? point.y - 1 : rPoint.y;
    res.z = point.z;
    return res;
}

Point3D Point3dUtils::getDownPoint(const Point3D &point, const Point3D &rPoint, const Point3D &lPoint) {
    Point3D res;
    res.x = point.x;
    res.y = point.y < rPoint.y ? point.y + 1 : lPoint.x;
    res.z = point.z;
    return res;
}

Point3D Point3dUtils::getLeftPoint(const Point3D &point, const Point3D &rPoint, const Point3D &lPoint) {
    Point3D res;
    res.x = point.x > lPoint.x ? point.x - 1 : rPoint.x;
    res.y = point.y;
    res.z = point.z;
    return res;
}

Point3D Point3dUtils::getRightPoint(const Point3D &point, const Point3D &rPoint, const Point3D &lPoint) {
    Point3D res;
    res.x = point.x < rPoint.x ? point.x + 1 : lPoint.x;
    res.y = point.y;
    res.z = point.z;
    return res;
}

Point3D Point3dUtils::getBeforePoint(const Point3D &point, const Point3D &rPoint, const Point3D &lPoint) {
    Point3D res;
    res.x = point.x;
    res.y = point.y;
    res.z = point.z > lPoint.z ? point.z - 1 : rPoint.z;
    return res;
}

Point3D Point3dUtils::getBehindPoint(const Point3D &point, const Point3D &rPoint, const Point3D &lPoint) {
    Point3D res;
    res.x = point.x;
    res.y = point.y;
    res.z = point.z < rPoint.z ? point.z + 1 : lPoint.z;
    return res;
}