//
//  Detector.hpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#ifndef Detector_hpp
#define Detector_hpp
#include <cv.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "FinderPoint.hpp"
using namespace std;
using namespace cv;
class Detetcor {
private:
    Mat& image;
    vector<FinderPoint> finderCenters;
    
    bool hasSkipped;
    bool checkRatio(int* stateCount);
    bool handlePossibleCenter(int* stateCount, int row, int col);
    float crossCheckVertical(int startI, int centerJ, int maxCount, int
                                       originalStateCountTotal);
    bool haveMultiplyConfirmedCenters();
    int getRowSkip();
public:
    vector<Point> pts;
    Detetcor(Mat& img);
    vector<FinderPoint>& get_points();
    bool find();
};
#endif /* Detector_hpp */
