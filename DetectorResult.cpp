//
// Created by 范志方 on 16/5/28.
//

#include "DetectorResult.h"

DetectorResult::DetectorResult(const vector<FinderPoint> &resultPoints, const BitMatrix& bits)  : resultPoints(
        resultPoints), bits(bits) {}

