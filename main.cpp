#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <iostream>

#include "Detector.h"
#include "Finder.hpp"
#include "Decoder.hpp"
//#include <string>
using namespace std;
using namespace cv;
int main(int argc, char** argv) {
    if (argc <= 1) {
        cerr << "Illegal argument" << endl;
        exit(1);
    }
    string file = argv[1];
    cout << file << endl;
    Mat img = imread(file);
    Mat imgBW;
    Mat testForDecode(200, 200, CV_8UC3, Scalar(255,255,255));
    cvtColor(img, imgBW, CV_BGR2GRAY);
    threshold(imgBW, imgBW, 128, 255, THRESH_BINARY);
    Finder finder = Finder(imgBW);
    FinderResult fr = finder.find();
    Detector detector = Detector(imgBW);
    DetectorResult detectorResult = detector.processFinderPatternInfo(fr);
//    test bits:
//    BitMatrix bits = detectorResult.getBits();
//    int dim = bits.getDimension();
//    for (int y = 0; y < dim; ++y) {
//        for (int x = 0; x < dim; ++x) {
//            if (bits.get(x,y)) {
//                rectangle(testForDecode,Point(5*x+15,5*y+15),Point(5*x+20,5*y+20),Scalar(0,0,0),-1);
//            }
//        }
//
//    }

//
    Decoder decoder = Decoder(detectorResult);
    decoder.decode();
    vector<FinderPoint> resultPoints = detectorResult.getResultPoints();
    for (auto it = resultPoints.begin(); it != resultPoints.end(); it++) {
        FinderPoint fp = *it;
        Point2f p = Point2f(fp.getX(), fp.getY());
        circle(img, p, 3, Scalar(0,255,0));
//        printf("(%f, %f)\n",p.x,p.y);
    }

    Point pTest = Point(41,52);
//    if (imgBW.at<uchar>(pTest) > 128) printf("ha\n");
//    circle(img, pTest, 2, Scalar(0,255,0));

    namedWindow("code", 1);
//    imshow("test.png",testForDecode);
    imshow("code.png", img);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findHorizonPattern.png", img);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findPattern.png", img);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/BW.png", imgBW);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/test.png", testForDecode);
    
    imshow("imgBW.png", imgBW);
    waitKey(0);
    return 0;
}