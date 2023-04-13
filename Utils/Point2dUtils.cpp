//
// Created by user on 10.04.23.
//

#include "Point2dUtils.hpp"


Point2D Point2dUtils::getUpPoint(const Point2D &point, const Point2D &rPoint, const Point2D &lPoint) {
    Point2D res;
    res.x = point.x;
    res.y = point.y > lPoint.y ? point.y - 1 : rPoint.y;
    return res;
}

Point2D Point2dUtils::getDownPoint(const Point2D &point, const Point2D &rPoint, const Point2D &lPoint) {
    Point2D res;
    res.x = point.x;
    res.y = point.y < rPoint.y ? point.y + 1 : lPoint.x;
    return res;
}

Point2D Point2dUtils::getLeftPoint(const Point2D &point, const Point2D &rPoint, const Point2D &lPoint) {
    Point2D res;
    res.x = point.x > lPoint.x ? point.x - 1 : rPoint.x;
    res.y = point.y;
    return res;
}

Point2D Point2dUtils::getRightPoint(const Point2D &point, const Point2D &rPoint, const Point2D &lPoint) {
    Point2D res;
    res.x = point.x < rPoint.x ? point.x + 1 : lPoint.x;
    res.y = point.y;
    return res;
}


