//
//  Finder.hpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#ifndef Finder_hpp
#define Finder_hpp
#include <cv.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "FinderPoint.hpp"
#include "FinderResult.hpp"
using namespace std;
using namespace cv;
class Finder {
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
    float distance(FinderPoint& p1, FinderPoint& p2);
    vector<FinderPoint> orderBestPatterns();
public:
    vector<Point2f> pts;
    Finder(Mat& img);
    vector<FinderPoint>& get_points();
    FinderResult find();
};
#endif /* Finder_hpp */
