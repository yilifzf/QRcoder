#include <cv.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <iostream>

#include "Detector.h"
#include "Finder.hpp"
#include "Decoder.hpp"
#include "QRcodeReader.h"
#include "Binarizer.h"
//#include <string>
using namespace std;
using namespace cv;
int main(int argc, char** argv) {
//
    if (argc <= 1) {
        cerr << "Illegal argument" << endl;
        exit(1);
    }
    bool more;
    int count = 1;

//    printf("%d\n",argc);
    if (argc == 2) {
        more = false;
    }
    if (argc == 3) {
        string option = argv[1];
        if (option.compare("--more") == 0) {
            more = true;
        }
        count++;
    }
    string filename = argv[count];
//    cout << filename << endl;
    Mat img = imread(filename);
//    namedWindow("original",2);

    QRcodeReader reader(img, more);
    reader.decode();
//    Mat gray;
//    cvtColor(img, gray, CV_BGR2GRAY);
//    imshow("orignial",gray);
//    Binarizer binarizer(gray);
//    gray = binarizer.getBlackMatrix();
//    imshow("after",gray);
//    waitKey(0);

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

//decode:


//    Point pTest = Point(116, 65);
////    if (imgBW.at<uchar>(pTest) > 128) printf("ha\n");
//    circle(imgBW, pTest, 2, Scalar(0,0,0));
//
//
////    imshow("test.png",testForDecode);
//    imshow("code.png", img);
////    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findHorizonPattern.png", img);
////    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/findPattern.png", img);
//    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/BW.png", imgBW);
////    imwrite("/Users/fzf_air/code/2016/QRcodeCPP/test.png", testForDecode);
//

//    imshow("test.png", testHist);

    return 0;
}