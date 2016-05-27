#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <iostream>

#include "Detector.h"
#include "Finder.hpp"
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
    cvtColor(img, imgBW, CV_BGR2GRAY);
    threshold(imgBW, imgBW, 128, 255, THRESH_BINARY);
    Finder finder = Finder(imgBW);
    FinderResult fr = finder.find();
    Detector detector = Detector(imgBW);
    detector.processFinderPatternInfo(fr);
    namedWindow("code", 1);
    for (auto it = finder.pts.begin(); it != finder.pts.end(); it++) {
        Point2f p = *it;
        circle(img, p, 3, Scalar(0,255,0));
    }
    for (auto it = detector.viewer.begin(); it != detector.viewer.end(); it++) {
        Point2f p2 = *it;
        circle(img, p2, 3, Scalar(255,0,0));
    }
    Point pTest = Point(160,155);
    circle(img, pTest, 2, Scalar(0,255,0));
    imshow("code.png", img);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findHorizonPattern.png", img);
    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findPattern.png", img);
    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/BW.png", imgBW);
    
//    imshow("imgBW.png", imgBW);
    waitKey(0);
    return 0;
}