//
// Created by 范志方 on 16/6/5.
//

#ifndef QRCODECPP_BINARIZER_H
#define QRCODECPP_BINARIZER_H

#include <cv.h>
#include <vector>
using namespace cv;
using namespace std;
class Binarizer {
    Mat source;
    vector<int> calculateBlackPoints(Mat &luminances, int subWidth, int subHeight, int width, int height);
    void calculateThresholdForBlock(Mat &luminances,
                                    int subWidth,
                                    int subHeight,
                                    int width,
                                    int height,
                                    vector<int> blackPoints);
    void thresholdBlock(Mat &luminances,
                        int xoffset,
                        int yoffset,
                        int threshold,
                        int stride);
public:
    Binarizer(const Mat &luminances) : source(luminances) { }
    Mat getBlackMatrix();
};


#endif //QRCODECPP_BINARIZER_H
