#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <iostream>

#include "Detector.hpp"
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
    Detetcor detector = Detetcor(imgBW);
    detector.find();
    namedWindow("code", 1);
    for (auto it = detector.pts.begin(); it != detector.pts.end(); it++) {
        Point p = *it;
        circle(img, p, 3, Scalar(0,255,0));
    }
    
    imshow("code.png", img);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findHorizonPattern.png", img);
    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findPattern.png", img);
    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/BW.png", imgBW);
    
//    imshow("imgBW.png", imgBW);
    waitKey(0);
    return 0;
}