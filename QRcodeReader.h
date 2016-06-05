//
// Created by 范志方 on 16/6/5.
//

#ifndef QRCODECPP_QRCODEREADER_H
#define QRCODECPP_QRCODEREADER_H

#include <cv.h>
#include <opencv2/opencv.hpp>
//#include "Detector/DetectorResult.h"
#include "Detector/Detector.h"
#include "Detector/Finder.hpp"
#include "Decoder/Decoder.hpp"

using namespace cv;

class QRcodeReader {
    Mat img;
    Mat rgbImg;
    bool more;
public:
    QRcodeReader(Mat &rgbImg, bool more);
    void decode();
};


#endif //QRCODECPP_QRCODEREADER_H
