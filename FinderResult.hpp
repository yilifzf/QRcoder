//
//  FinderResult.hpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/25.
//  Copyright © 2016年 范志方. All rights reserved.
//

#ifndef FinderResult_hpp
#define FinderResult_hpp
#include <vector>
#include "FinderPoint.hpp"
#include <stdio.h>
using namespace std;
class FinderResult {
private:
    FinderPoint topLeft;
    FinderPoint topRight;
    FinderPoint bottomLeft;
public:
    FinderResult(vector<FinderPoint> pts);

    const FinderPoint &getTopLeft() const {
        return topLeft;
    }

    const FinderPoint &getTopRight() const {
        return topRight;
    }

    const FinderPoint &getBottomLeft() const {
        return bottomLeft;
    }
};
#endif /* FinderResult_hpp */
