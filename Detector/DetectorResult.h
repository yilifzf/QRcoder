//
// Created by 范志方 on 16/5/28.
//

#ifndef QRCODECPP_DETECTORRESULT_H
#define QRCODECPP_DETECTORRESULT_H
#include <vector>
#include "FinderPoint.hpp"
#include "BitMatrix.h"

using namespace std;
class DetectorResult {
private:
    vector<FinderPoint> resultPoints;
    BitMatrix bits;
    float moduleSize;
    int dimension;
public:
    float getModuleSize() const {
        return moduleSize;
    }

    int getDimension() const {
        return dimension;
    }

    DetectorResult(const vector<FinderPoint> resultPoints, const BitMatrix &bits, int moduleSize, int dimension);

    vector<FinderPoint> getResultPoints() const {
        return resultPoints;
    }

    BitMatrix getBits() const {
        return bits;
    }
};


#endif //QRCODECPP_DETECTORRESULT_H
