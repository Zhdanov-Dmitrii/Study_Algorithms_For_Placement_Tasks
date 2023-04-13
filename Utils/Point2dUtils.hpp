//
// Created by user on 10.04.23.
//

#ifndef DIPLOMA_POINT2DUTILS_HPP
#define DIPLOMA_POINT2DUTILS_HPP

#include "Points/Point2D.hpp"

struct Point2dUtils {
    static Point2D getUpPoint (const Point2D& point, const Point2D& rPoint, const Point2D& lPoint = {0,0});
    static Point2D getDownPoint (const Point2D& point, const Point2D& rPoint, const Point2D& lPoint = {0,0});
    static Point2D getLeftPoint (const Point2D& point, const Point2D& rPoint, const Point2D& lPoint = {0,0});
    static Point2D getRightPoint (const Point2D& point, const Point2D& rPoint, const Point2D& lPoint = {0,0});

};


#endif //DIPLOMA_POINT2DUTILS_HPP
