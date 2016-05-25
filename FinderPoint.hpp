//
//  FinderPoint.hpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#ifndef FinderPoint_hpp
#define FinderPoint_hpp
#include <stdlib.h>
#include <stdio.h>
class FinderPoint {
private:
    const float _x;
    const float _y;
public:
    FinderPoint();
    FinderPoint(float x, float y);
    float get_x();
    float get_y();
    bool aboutEquals(float moduleSize, float i, float j);
};
#endif /* FinderPoint_hpp */
