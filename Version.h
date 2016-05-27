//
// Created by 范志方 on 16/5/26.
//

#ifndef QRCODECPP_VERSION_H
#define QRCODECPP_VERSION_H
#include <cv.h>
#include <vector>
#include "FinderPoint.hpp"

using namespace cv;
class Version {
private:
    Mat& image;
    vector<FinderPoint> AlignmentPoints;
    int versionNumber;
    bool checkRatio(vector<int> stateCount, float moduleSize);
    bool handlePossibleCenter(vector<int> stateCount, int i, int j, float moduleSize);
    float crossCheckVertical(int startI, int centerJ, int maxCount, int originalStateCountTotal, float moduleSize);
public:
    Version(Mat& img, int dimension);
    int getDimensionForVersion();
    vector<FinderPoint> getAlignmentPatternCenters();
    int getVersionNumber();
    bool findAlignmentInRegion(float overallEstModuleSize, int estAlignmentX, int estAlignmentY,
                                    float allowanceFactor);
};


#endif //QRCODECPP_VERSION_H
