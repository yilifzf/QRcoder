//
// Created by 范志方 on 16/5/28.
//

#include "DetectorResult.h"

DetectorResult::DetectorResult(const vector<FinderPoint> resultPoints, const BitMatrix& bits, int moduleSize, int dimension)  : resultPoints(
        resultPoints), bits(bits) {}

