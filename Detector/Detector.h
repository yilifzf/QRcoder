//
// Created by 范志方 on 16/5/25.
//

#ifndef QRCODECPP_DETECTOR_H
#define QRCODECPP_DETECTOR_H

#include <cv.h>
#include "FinderResult.hpp"
#include "Version.h"
#include "DetectorResult.h"

using namespace cv;
class Detector {
private:
    Mat& image;
public:
    Detector(Mat& image);

private:
    float calculateModuleSize(FinderPoint& topLeft, FinderPoint& topRight, FinderPoint& bottomLeft);
    int computeDimension(FinderPoint& topLeft, FinderPoint& topRight, FinderPoint& bottomLeft, float moduleSize);
    float calculateModuleSizeOneWay(FinderPoint& pattern, FinderPoint& otherPattern);
    float sizeOf11311Pattern(int fromX, int fromY, int toX, int toY);
    void RawSampleGrid(FinderPoint topLeft, FinderPoint topRight, FinderPoint bottomLeft,
                       FinderPoint alignment, vector<bool> &result);
    Point2f transform(float x, float y, FinderPoint topLeft, FinderPoint topRight, FinderPoint bottomLeft,
                      FinderPoint alignment, int modulesBetweenFinderCenter);
    Mat getTransform(vector<FinderPoint> &input, int dimension, int moduleSize);
    Mat getReverseTransform(vector<FinderPoint> &input, int dimension, float moduleSize);
    void sampleGrid(Mat &transform, vector<bool> &result, float moduleSize);

public:
    vector<Point> viewer;
    DetectorResult processFinderPatternInfo(FinderResult fr);
};


#endif //QRCODECPP_DETECTOR_H
