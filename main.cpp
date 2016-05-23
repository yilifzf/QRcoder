#include <cv.h>
#include <highgui.h>
#include <iostream>
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
    Mat src = imread(file);
    flip(src.t(), src, 1);
    namedWindow("code", 1);
    imshow("code.png", src);
    waitKey(0);
    return 0;
}