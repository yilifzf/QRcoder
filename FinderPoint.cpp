//
//  FinderPoint.cpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#include "FinderPoint.hpp"
FinderPoint::FinderPoint():_x(0), _y(0) {
}

FinderPoint::FinderPoint(float x, float y) : _x(x), _y(y) {
}

float FinderPoint::get_x() {
    return _x;
}
float FinderPoint::get_y() {
    return _y;
}
bool FinderPoint::aboutEquals(float moduleSize, float i, float j) {
    return abs(i - _y) <= moduleSize && abs(j - _x) <= moduleSize;
}
