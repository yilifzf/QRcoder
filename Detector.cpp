//
// Created by 范志方 on 16/5/25.
//

#include "Detector.h"

Detector::Detector(Mat& img) : image(img){
}

void Detector::processFinderPatternInfo(FinderResult fr) {
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
    Version version = Version(image, dimension);
    int modulesBetweenFPCenters = version.getDimensionForVersion() - 7;

    vector<FinderPoint> alignmentPattern;
    vector<bool> bitMatrix(dimension*dimension, false);
    if(version.getVersionNumber() > 1) {

        float transform = topRight.getX() - topLeft.getX() + bottomLeft.getX();
        float bits = topRight.getY() - topLeft.getY() + bottomLeft.getY();
        float points = 1.0F - 3.0F / (float)modulesBetweenFPCenters;
        int estAlignmentX = (int)(topLeft.getX() + points * (transform - topLeft.getX()));
        int estAlignmentY = (int)(topLeft.getY() + points * (bits - topLeft.getY()));
        int i = 4;

        while(i <= 16) {
            if (version.findAlignmentInRegion(moduleSize, estAlignmentX, estAlignmentY, (float)i)) {
                alignmentPattern = version.getAlignmentPatternCenters();
                break;
            }
            else {
                i <<= 1;
            }
        }
    }

    RawSampleGrid(topLeft, topRight, bottomLeft, bitMatrix);
//    if (bitMatrix[0]) {
//                printf("X\n");
//    }

    //show for debug
    for (int iy = 0; iy < dimension; ++iy) {
        int jx = 0;
        for (; jx < dimension; ++jx) {
            if (bitMatrix[iy*dimension + jx]) {
                printf("X");
            }
            else {
                printf(" ");
            }
            if (jx == dimension - 1) {
                printf("\n");
            }
        }
    }
    ///////
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

void Detector::RawSampleGrid(FinderPoint topLeft, FinderPoint topRight, FinderPoint bottomLeft, vector<bool> &result) {
    int dimension = sqrt(result.size());
    int modulesBetweenFinderCenter = dimension - 7;
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            Point2f tmp = transform(j + 0.5f, i + 0.5f, topLeft, topRight, bottomLeft, modulesBetweenFinderCenter);
            if (image.at<uchar>(tmp) < 128) {
                result[i*dimension + j] = true;
            } else {
                result[i*dimension + j] = false;
            }
        }
    }
}


Point2f Detector::transform(float x, float y, FinderPoint topLeft, FinderPoint topRight, FinderPoint bottomLeft,
                          int modulesBetweenFinderCenter) {
    Point2f zero = Point2f(topLeft.getX(),topLeft.getY());
    Point2f tR = Point2f(topRight.getX(),topRight.getY());
    Point2f bL = Point2f(bottomLeft.getX(),bottomLeft.getY());
    Point2f xBase = Point2f((tR - zero).x/modulesBetweenFinderCenter, (tR - zero).y/modulesBetweenFinderCenter);
    Point2f yBase = Point2f((bL - zero).x/(float)modulesBetweenFinderCenter, (bL - zero).y/(float)modulesBetweenFinderCenter);
    Point2f newX = xBase * (x - 3.5);
    Point2f newY = yBase * (y - 3.5);
    return  newX + newY + zero;
}
