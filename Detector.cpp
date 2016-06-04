//
// Created by 范志方 on 16/5/25.
//

#include <opencv2/highgui/highgui.hpp>
#include "Detector.h"

Detector::Detector(Mat& img) : image(img){
}

DetectorResult Detector::processFinderPatternInfo(FinderResult fr) {
//    This function calculate the module size and find the alignment center,
// finally it transform the black and white modules to a binary matrix.
    FinderPoint topLeft = fr.getTopLeft();
    FinderPoint topRight = fr.getTopRight();
    FinderPoint bottomLeft = fr.getBottomLeft();
    float moduleSize = calculateModuleSize(topLeft, topRight, bottomLeft);
    if (moduleSize < 1.0f) {
        printf("bad module size\n");
        exit(1);
    }
    printf("ModuleSize : %f\n", moduleSize);
    int dimension = computeDimension(topLeft, topRight, bottomLeft, moduleSize);
    printf("dimension : %d\n", dimension);
    Version version = Version(dimension);
    int modulesBetweenFPCenters = version.getDimensionForVersion() - 7;

    vector<FinderPoint> alignmentPattern;
    vector<bool> bitMatrix(dimension*dimension, false);
    if(version.getVersionNumber() > 1) {

        float transform = topRight.getX() - topLeft.getX() + bottomLeft.getX();
        float bits = topRight.getY() - topLeft.getY() + bottomLeft.getY();
        float points = 1.0F - 3.0F / (float)modulesBetweenFPCenters;

        // We can calculate where the alignment center should be.
        int estAlignmentX = (int)(topLeft.getX() + points * (transform - topLeft.getX()));
        int estAlignmentY = (int)(topLeft.getY() + points * (bits - topLeft.getY()));
        int i = 4;

        while(i <= 16) {
            if (version.findAlignmentInRegion(image, moduleSize, estAlignmentX, estAlignmentY, (float)i)) {
                alignmentPattern = version.getAlignmentPatternCenters();
                break;
            }
            else {
                // try to search in a wider range.
                i <<= 1;
            }
        }
    }
    vector<FinderPoint> resultPoints;
    resultPoints.push_back(bottomLeft);
    resultPoints.push_back(topLeft);
    resultPoints.push_back(topRight);
    for (auto it = alignmentPattern.begin(); it != alignmentPattern.end(); ++it) {
        resultPoints.push_back(*it);
    }
//    printf("rs:%d\n",resultPoints.size());
    Mat transform = getTransform(resultPoints, dimension, moduleSize);
    Mat reverseTransform = getReverseTransform(resultPoints, dimension, moduleSize);
    Mat output;
    warpPerspective(image,output,transform,output.size());
    cv::imshow("transform", output);


    // test:
//    vector<Point2f> test;
//    test.push_back(Point2f(alignmentPattern[0].getX(), alignmentPattern[0].getY()));
//    vector<Point2f> testOut;
//    perspectiveTransform(test,testOut,reverseTranform);
//    printf("(%f, %f)\n", testOut[0].x, testOut[0].y);
//    circle(output, testOut[0], 2, Scalar(255,255,255));

    //

    // Now we sample the image to a matrix.
    sampleGrid(reverseTransform, bitMatrix, moduleSize);
//    RawSampleGrid(topLeft, topRight, bottomLeft, alignmentPattern[0], bitMatrix);

//    if (bitMatrix[0]) {
//                printf("X\n");
//    }

    //show for debug
    ///////
    BitMatrix bits = BitMatrix(bitMatrix, dimension);
    bits.display();

    DetectorResult result = DetectorResult(resultPoints, bits);
    return result;
}

float Detector::calculateModuleSize(FinderPoint& topLeft, FinderPoint& topRight, FinderPoint& bottomLeft) {
    return (calculateModuleSizeOneWay(topLeft, topRight) + calculateModuleSizeOneWay(topLeft, bottomLeft)) / 2.0f;
}

float Detector::calculateModuleSizeOneWay(FinderPoint &pattern, FinderPoint &otherPattern) {
    //For simplification, just compute the size in pattern, omitting the otherPattern
    float moduleSizeEst1 = sizeOf11311Pattern((int)pattern.getX(), (int)pattern.getY(),
                                                            (int)otherPattern.getX(), (int)otherPattern.getY());
    float moduleSizeEst2 = sizeOf11311Pattern((int)otherPattern.getX(), (int)otherPattern.getY(),
                                              (int)pattern.getX(), (int)pattern.getY());
    return (moduleSizeEst1 + moduleSizeEst2)/14.0;
}

float Detector::sizeOf11311Pattern(int fromX, int fromY, int toX, int toY) {
    // Mild variant of Bresenham's algorithm;
    // see http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
    float result = 0;
    bool steep = abs(toY - fromY) > abs(toX - fromX);
    if (steep) {
        int temp = fromX;
        fromX = fromY;
        fromY = temp;
        temp = toX;
        toX = toY;
        toY = temp;
    }

    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);
    int error = -dx >> 1;
    int xstep = fromX < toX ? 1 : -1;
    int ystep = fromY < toY ? 1 : -1;

    // In black pixels, looking for white, first or second time.
    int state = 0;
    // Loop up until x == toX, but not beyond
    int xLimit = toX + xstep;
    for (int x = fromX, y = fromY; x != xLimit; x += xstep) {
        int realX = steep ? y : x;
        int realY = steep ? x : y;
//        if (!steep) {
//            viewer.push_back(Point(x, y));
//        } else {
//            viewer.push_back(Point(y, x));
//        }

        // Does current pixel mean we have moved white to black or vice versa?
        if (!((state == 1) ^ (image.at<uchar>(Point(realX, realY)) < 128))) {
            if (state == 2) {
                float xDiff = abs(x - fromX);
                float yDiff = abs(y - fromY);
                result = sqrt(xDiff * xDiff + yDiff * yDiff);
                break;
            }
            state++;
        }

        error += dy;
        if (error > 0) {
            if (y == toY) {
                break;
            }
            y += ystep;
            error -= dx;
        }
    }


    //turn around to count the other half part.
    state = 0;
    xstep = - xstep;
    ystep = -ystep;
    for (int x = fromX + xstep, y = fromY; x != 0 && x != image.cols; x += xstep) {
        int realX = steep ? y : x;
        int realY = steep ? x : y;
//        if (!steep) {
//            viewer.push_back(Point(x, y));
//        } else {
//            viewer.push_back(Point(y, x));
//        }
        // Does current pixel mean we have moved white to black or vice versa?
        if (!((state == 1) ^ (image.at<uchar>(Point(realX, realY)) < 128))) {
            if (state == 2) {
                float xDiff = abs(x - fromX);
                float yDiff = abs(y - fromY);
                result += sqrt(xDiff * xDiff + yDiff * yDiff);
                return result - 1.0;
            }
            state++;
        }

        error += dy;
        if (error > 0) {
            if (y == toY) {
                break;
            }
            y += ystep;
            error -= dx;
        }
    }

    // else we didn't find even black-white-black; no estimate is really possible
    return 0.0/0.0;
}

int Detector::computeDimension(FinderPoint& topLeft, FinderPoint& topRight, FinderPoint& bottomLeft, float moduleSize) {
    int tltrCentersDimension =
            (int)round(topLeft.distance(topRight) / moduleSize);
    int tlblCentersDimension =
            (int)round(topLeft.distance(bottomLeft) / moduleSize);
    int dimension = ((tltrCentersDimension + tlblCentersDimension) >> 1) + 7;
    switch (dimension & 0x03) { // mod 4
        case 0:
            dimension++;
            break;
            // 1? do nothing
        case 2:
            dimension--;
            break;
        case 3:
            printf("Bad dimension\n");
            exit(1);
    }
    return dimension;
}

void Detector::RawSampleGrid(FinderPoint topLeft, FinderPoint topRight, FinderPoint bottomLeft,
                             FinderPoint alignment, vector<bool> &result) {
    int dimension = sqrt(result.size());
    int modulesBetweenFinderCenter = dimension - 7;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            Point2f tmp = transform(j + 0.5f, i + 0.5f, topLeft, topRight, bottomLeft, alignment, modulesBetweenFinderCenter);
            if (image.at<uchar>(tmp) < 128) {
                result[i*dimension + j] = true;
            } else {
                result[i*dimension + j] = false;
            }
        }
    }
}


Point2f Detector::transform(float x, float y, FinderPoint topLeft, FinderPoint topRight, FinderPoint bottomLeft,
                            FinderPoint alignment, int modulesBetweenFinderCenter) {
    if (x < modulesBetweenFinderCenter && y < modulesBetweenFinderCenter) {
        Point2f zero = Point2f(topLeft.getX(),topLeft.getY());
        Point2f tR = Point2f(topRight.getX(),topRight.getY());
        Point2f bL = Point2f(bottomLeft.getX(),bottomLeft.getY());
        Point2f xBase = Point2f((tR - zero).x/(float)modulesBetweenFinderCenter, (tR - zero).y/(float)modulesBetweenFinderCenter);
        Point2f yBase = Point2f((bL - zero).x/(float)modulesBetweenFinderCenter, (bL - zero).y/(float)modulesBetweenFinderCenter);
        Point2f newX = xBase * (x - 3.5);
        Point2f newY = yBase * (y - 3.5);
        return  newX + newY + zero;
    }
    else {
        Point2f Tl = Point2f(topLeft.getX(),topLeft.getY());
        Point2f Tr = Point2f(topRight.getX(),topRight.getY());
        Point2f Bl = Point2f(bottomLeft.getX(),bottomLeft.getY());
        Point2f Xbase = Point2f((Tr - Tl).x/(float)modulesBetweenFinderCenter, (Tr - Tl).y/(float)modulesBetweenFinderCenter);
        Point2f Ybase = Point2f((Bl - Tl).x/(float)modulesBetweenFinderCenter, (Bl - Tl).y/(float)modulesBetweenFinderCenter);

        Point2f zero = Point2f(alignment.getX(),alignment.getY());
        Point2f TrNear = Tr - 3*Xbase;
        Point2f BlNear = Bl - 3*Ybase;
        Point2f yBase = Point2f((zero - TrNear).x/(float)(modulesBetweenFinderCenter - 3), -(TrNear - zero).y/(float)(modulesBetweenFinderCenter - 3));
        Point2f xBase = Point2f(-(BlNear - zero).x/(float)(modulesBetweenFinderCenter - 3), -(BlNear - zero).y/(float)(modulesBetweenFinderCenter - 3));
        Point2f newX = xBase * (x - modulesBetweenFinderCenter - 0.5);
        Point2f newY = yBase * (y - modulesBetweenFinderCenter - 0.5);
        return  newX + newY + zero;
    }
}

Mat Detector::getTransform(vector<FinderPoint> &input, int dimension, int moduleSize) {
    vector<Point2f> src;
    for (int i = 0; i < 4; ++i) {
        Point2f p =  Point2f(input[i].getX(), input[i].getY());
        src.push_back(p);
    }
    vector<Point2f> dst;
    dst.push_back(Point2f(5 * moduleSize, dimension * moduleSize - 5 * moduleSize));
    dst.push_back(Point2f(5 * moduleSize, 5 * moduleSize));
    dst.push_back(Point2f(dimension * moduleSize - 5 * moduleSize, 5 * moduleSize));
    dst.push_back(Point2f(dimension * moduleSize - 8 * moduleSize, dimension * moduleSize - 8 * moduleSize));
    Mat result = getPerspectiveTransform(src, dst);
    return result;
}

Mat Detector::getReverseTransform(vector<FinderPoint> &input, int dimension, float moduleSize) {
    vector<Point2f> src;
    for (int i = 0; i < 4; ++i) {
        Point2f p =  Point2f(input[i].getX(), input[i].getY());
        src.push_back(p);
    }
    vector<Point2f> dst;
    dst.push_back(Point2f(3.5 * moduleSize, dimension * moduleSize - 3.5 * moduleSize));
    dst.push_back(Point2f(3.5 * moduleSize, 3.5 * moduleSize));
    dst.push_back(Point2f(dimension * moduleSize - 3.5 * moduleSize, 3.5 * moduleSize));
    dst.push_back(Point2f(dimension * moduleSize - 6.5 * moduleSize, dimension * moduleSize - 6.5 * moduleSize));
//    printf("(%f, %f)\n", dst[1].x, dst[1].y);
    Mat result = getPerspectiveTransform(dst, src);
    return result;
}

void Detector::sampleGrid(Mat &transform, vector<bool> &result, float moduleSize) {
    int dimension = sqrt(result.size());
    int modulesBetweenFinderCenter = dimension - 7;
    vector<Point2f> pointDst, pointSrc;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            Point2f dst((j+0.5)*moduleSize, (i+0.5)*moduleSize);
            pointDst.push_back(dst);
        }
    }
    perspectiveTransform(pointDst, pointSrc, transform);
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            Point2f src = pointSrc[i * dimension + j];
            if (image.at<uchar>(src) < 128) {
                result[i*dimension + j] = true;
            } else {
                result[i*dimension + j] = false;
            }
        }
    }

}
