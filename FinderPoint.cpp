//
//  FinderPoint.cpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#include <math.h>
#include "FinderPoint.hpp"
FinderPoint::FinderPoint():_x(0), _y(0) {
}

FinderPoint::FinderPoint(float x, float y) : _x(x), _y(y) {
}

FinderPoint::FinderPoint(float x, float y, float estimatedModuleSize) : _x(x), _y(y), _estimatedModuleSize(estimatedModuleSize) {
}

float FinderPoint::getX() {
    return _x;
}
float FinderPoint::getY() {
    return _y;
}
bool FinderPoint::aboutEquals(float moduleSize, float i, float j) {
    return abs(i - _y) <= moduleSize && abs(j - _x) <= moduleSize;
}

float FinderPoint::distance(FinderPoint p) {
    float xDiff = abs(this->_x - p.getX());
    float yDiff = abs(this->_y - p.getY());
    return sqrt(xDiff*xDiff + yDiff*yDiff);
}

float FinderPoint::getEstimatedModuleSize() {
    return _estimatedModuleSize;
}
