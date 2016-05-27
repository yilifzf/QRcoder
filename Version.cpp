//
// Created by 范志方 on 16/5/26.
//

#include "Version.h"
using namespace std;
Version::Version(Mat& img, int dimension) : image(img){
    if(dimension % 4 != 1) {
        printf("Bad version\n");
        exit(1);
    } else {
        versionNumber =  (dimension - 17) / 4;
        int ptsNum;
        if (versionNumber == 1) { ptsNum = 0;}
        else {
            ptsNum = (versionNumber/7 + 2) * (versionNumber/7 + 2) -3;
        }
        AlignmentPoints = vector<FinderPoint>(ptsNum);
    }
}

int Version::getDimensionForVersion() {
    return versionNumber * 4 + 17;
}

vector<FinderPoint> Version::getAlignmentPatternCenters() {
    return this->AlignmentPoints;
}

int Version::getVersionNumber() {
    return versionNumber;
}

bool Version::findAlignmentInRegion(float moduleSize, int estAlignmentX, int estAlignmentY,
                                    float allowanceFactor) {
    int allowance = (int)(allowanceFactor * moduleSize);
    int startX = max(0, estAlignmentX - allowance);
    int endX = min((int)(image.cols - 1), estAlignmentX + allowance);
    int width = endX - startX;
    int startY = max(0, estAlignmentY - allowance);
    int endY = min((int)(image.rows - 1), estAlignmentY + allowance);
    int height = endY - startY;
    int middleI = startY + (height >> 1);
    int maxJ = endX;
    vector<int> stateCount(3, 0);
    for (int iGen = 0; iGen < height; iGen++) {
        // Search from middle outwards
        int i = middleI + ((iGen & 0x01) == 0 ? ((iGen + 1) >> 1) : -((iGen + 1) >> 1));
        //        image_->getBlackRow(i, luminanceRow, startX_, width_);
        stateCount[0] = 0;
        stateCount[1] = 0;
        stateCount[2] = 0;
        int j = startX;
        // Burn off leading white pixels before anything else; if we start in the middle of
        // a white run, it doesn't make sense to count its length, since we don't know if the
        // white run continued to the left of the start point
        while (j < maxJ && !image.at<uchar>(Point(j, i))) {
            j++;
        }
        int currentState = 0;
        while (j < maxJ) {
            if (image.at<uchar>(Point(j, i)) < 128) {
                // Black pixel
                if (currentState == 1) { // Counting black pixels
                    stateCount[currentState]++;
                } else { // Counting white pixels
                    if (currentState == 2) { // A winner?
                        if (checkRatio(stateCount, moduleSize)) { // Yes
                            bool confirmed = handlePossibleCenter(stateCount, i, j, moduleSize);
                            if (confirmed) {
                                return true;
                            }
                        }
                        stateCount[0] = stateCount[2];
                        stateCount[1] = 1;
                        stateCount[2] = 0;
                        currentState = 1;
                    } else {
                        stateCount[++currentState]++;
                    }
                }
            } else { // White pixel
                if (currentState == 1) { // Counting black pixels
                    currentState++;
                }
                stateCount[currentState]++;
            }
            j++;
        }
        if (checkRatio(stateCount, moduleSize)) {
            bool confirmed = handlePossibleCenter(stateCount, i, maxJ, moduleSize);
            if (confirmed) {
                return true;
            }
        }

    }
    return false;
}

bool Version::checkRatio(vector<int> stateCount, float moduleSize) {
    float maxVariance = moduleSize / 2.0f;
    for (int i = 0; i < 3; i++) {
        if (abs(moduleSize - stateCount[i]) >= maxVariance) {
            return false;
        }
    }
    return true;
}

bool Version::handlePossibleCenter(vector<int> stateCount, int i, int j, float moduleSize) {
    int stateCountTotal = stateCount[0] + stateCount[1] + stateCount[2];
    float centerJ = (float)(j - stateCount[2]) - stateCount[1] / 2.0f;
    float centerI = crossCheckVertical(i, (int) centerJ, 2 * stateCount[1], stateCountTotal, moduleSize);
    if (!isnan(centerI)) {
//        leave for later extend

//        float estimatedModuleSize = (float)(stateCount[0] + stateCount[1] + stateCount[2]) / 3.0f;
//        int max = possibleCenters_->size();
//        for (int index = 0; index < max; index++) {
//            Ref<AlignmentPattern> center((*possibleCenters_)[index]);
//            // Look for about the same center and module size:
//            if (center->aboutEquals(estimatedModuleSize, centerI, centerJ)) {
//                return center->combineEstimate(centerI, centerJ, estimatedModuleSize);
//            }
//        }
//        AlignmentPattern *tmp = new AlignmentPattern(centerJ, centerI, estimatedModuleSize);
//        // Hadn't found this before; save it
//        tmp->retain();
//        possibleCenters_->push_back(tmp);
//        if (callback_ != 0) {
//            callback_->foundPossibleResultPoint(*tmp);
//        }
        FinderPoint newCenter = FinderPoint(centerJ, centerI);
        AlignmentPoints.push_back(FinderPoint(centerJ, centerI));
        printf("Created new center: (%f, %f)\n", newCenter.getX(), newCenter.getY());
        return true;
    }
    return false;
}

float Version::crossCheckVertical(int startI, int centerJ, int maxCount, int originalStateCountTotal, float moduleSize) {
    int maxI = image.rows;
    vector<int> stateCount(3,0);
//    for (int i = 0; i < 3; i++) {
//        stateCount[i] = 0;
//    }
    int i = startI;
    //1. count black up from center
    while (i >= 0 && image.at<uchar>(Point(centerJ, i)) < 128) {
        stateCount[1]++;
        i--;
    }
    //continue count white
    while (i >= 0 && image.at<uchar>(Point(centerJ, i)) > 128) {
        stateCount[0]++;
        i--;
    }

//    if (stateCount[0] + stateCount[1] > maxCount) {
//        return 0.0/0.0;
//    }

    //2. count black down from center
    i = startI + 1;
    while (i < maxI && image.at<uchar>(Point(centerJ, i)) < 128) {
        stateCount[1]++;
        i++;
    }
    while (i < maxI && image.at<uchar>(Point(centerJ, i)) > 128) {
        stateCount[2]++;
        i++;
    }
    if (stateCount[0] > maxCount || stateCount[1] > maxCount) {
        return 0.0/0.0;
    }

    return checkRatio(stateCount, moduleSize) ? (float)(i - stateCount[2]) - stateCount[1] / 2.0f : 0.0/0.0;
}
