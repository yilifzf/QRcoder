//
//  Finder.cpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#include "Finder.hpp"
Detetcor::Detetcor(Mat& img) : image(img), hasSkipped(false){
}

vector<FinderPoint>& Detetcor::get_points() {
    return finderCenters;
}

bool Detetcor::find() {
    bool done = false;
    int skipRows = 3;
    int stateCount[5] = {0};
    int currentState = 0;
    for(int row = skipRows - 1; row < image.rows; row += skipRows)
    {
        stateCount[0] = 0;
        stateCount[1] = 0;
        stateCount[2] = 0;
        stateCount[3] = 0;
        stateCount[4] = 0;
        currentState = 0;
        uchar* ptr = image.ptr<uchar>(row);
        for(int col=0; col < image.cols; col++)
        {
            if(ptr[col] < 128)
            {
                // We're at a black pixel
                if((currentState & 0x1)==1)
                {
                    // We were counting white pixels
                    // So change the state now
                    
                    // W->B transition
                    currentState++;
                }
                
                // Works for boths W->B and B->B
                stateCount[currentState]++;
            }
            else
            {
                // We got to a white pixel...
                if((currentState & 0x1)==1)
                {
                    // W->W change
                    stateCount[currentState]++;
                }
                else
                {
                    // ...but, we were counting black pixels
                    if(currentState==4)
                    {
                        // We found the 'white' area AFTER the finder patter
                        // Do processing for it here
                        if(checkRatio(stateCount))
                        {
                            // This is where we do some more checks
                            bool confirmed = handlePossibleCenter(stateCount, row, col);
                            if(confirmed) {
                                skipRows = 2;
                                if(hasSkipped) {
                                    done = haveMultiplyConfirmedCenters();
                                }
                                else {
                                    int rowsToSkip = getRowSkip();
                                    if(rowsToSkip > stateCount[2]) {
                                        row += rowsToSkip - stateCount[2] - skipRows;
                                        col = image.cols - 1;
                                    }
                                }
                            }
                            else {
                                stateCount[0] = stateCount[2];
                                stateCount[1] = stateCount[3];
                                stateCount[2] = stateCount[4];
                                stateCount[3] = 1;
                                stateCount[4] = 0;
                                currentState = 3;
                                continue;
                            }
                            
                            // Reset all counts and continue looking for more
                            // finder patterns in the current row
                            currentState = 0;
                            stateCount[0] = 0;
                            stateCount[1] = 0;
                            stateCount[2] = 0;
                            stateCount[3] = 0;
                            stateCount[4] = 0;
                            
                        }
                        else
                        {
                            currentState = 3;
                            stateCount[0] = stateCount[2];
                            stateCount[1] = stateCount[3];
                            stateCount[2] = stateCount[4];
                            stateCount[3] = 1;
                            stateCount[4] = 0;
                            continue;
                        }
                        currentState = 0;
                        stateCount[0] = 0;
                        stateCount[1] = 0;
                        stateCount[2] = 0;
                        stateCount[3] = 0;
                        stateCount[4] = 0;
                    }
                    else
                    {
                        // We still haven't go 'out' of the finder pattern yet
                        // So increment the state
                        // B->W transition
                        currentState++;
                        stateCount[currentState]++;
                    }
                }
            }
        }
    }
    
    return false;
}
bool Detetcor::checkRatio(int* stateCount) {
    int totalFinderSize = 0;
    for(int i=0;i<5;i++) {
        int count = stateCount[i];
        totalFinderSize += count;
        if(count==0)
            return false;
    }
    
    if(totalFinderSize<7) {
        return false;
    }
    
    // Calculate the size of one module
    int moduleSize = ceil(totalFinderSize / 7.0);
    // Consider the variance owing to distortion
    int maxVariance = moduleSize/2;
    
    bool retVal = ((abs(moduleSize - stateCount[0])) < maxVariance &&
                   (abs(moduleSize - stateCount[1])) < maxVariance &&
                   (abs(3*moduleSize - stateCount[2])) < 3*maxVariance &&
                   (abs(moduleSize - stateCount[3])) < maxVariance &&
                   (abs(moduleSize - stateCount[4])) < maxVariance);
    return retVal;
}
bool Detetcor::handlePossibleCenter(int* stateCount, int row, int col) {
    int stateCountTotal = stateCount[0] + stateCount[1] + stateCount[2] + stateCount[3] + stateCount[4];
    float centerJ = (float)(col - stateCount[4] - stateCount[3]) - stateCount[2] / 2.0f;
    float centerI = crossCheckVertical(row, (int)centerJ, stateCount[2], stateCountTotal);
    if (!isnan(centerI)) {
        float estimatedModuleSize = (float)stateCountTotal / 7.0f;
        bool found = false;
        for (auto it = finderCenters.begin(); it != finderCenters.end(); it++) {
            FinderPoint& center = *it;
            if (center.aboutEquals(estimatedModuleSize, centerI, centerJ)) {
                found = true;
                break;
            }
        }
        if (!found) {
            FinderPoint newCenter = FinderPoint(centerJ, centerI);
            pts.push_back(Point(centerJ, centerI));
            printf("Created new center: (%f, %f)\n", newCenter.get_x(), newCenter.get_y());
            finderCenters.push_back(newCenter);
        }
        return true;
    }
    return false;
}

float Detetcor::crossCheckVertical(int startI, int centerJ, int maxCount, int
                                   originalStateCountTotal) {
    int maxI = image.rows;
    int stateCount[5];
    for (int i = 0; i < 5; i++) {
        stateCount[i] = 0;
    }
    int i = startI;
    //1. count black up from center
    while (i >= 0 && image.at<uchar>(Point(centerJ, i)) < 128) {
        stateCount[2]++;
        i--;
    }
    //continue count white
    while (i >= 0 && image.at<uchar>(Point(centerJ, i)) > 128) {
        stateCount[1]++;
        i--;
    }
    //the toppest black
    while (i >= 0 && image.at<uchar>(Point(centerJ, i)) < 128) {
        stateCount[0]++;
        i--;
    }
    if (stateCount[0] + stateCount[1] > maxCount) {
        return 0.0/0.0;
    }
    //2. count black down from center
    i = startI + 1;
    while (i < maxI && image.at<uchar>(Point(centerJ, i)) < 128) {
        stateCount[2]++;
        i++;
    }
    while (i < maxI && image.at<uchar>(Point(centerJ, i)) > 128) {
        stateCount[3]++;
        i++;
    }
    while (i < maxI && image.at<uchar>(Point(centerJ, i)) < 128) {
        stateCount[4]++;
        i++;
    }
    if (stateCount[0] + stateCount[1] > maxCount) {
        return 0.0/0.0;
    }
    
    return checkRatio(stateCount) ? (float)(i - stateCount[4] - stateCount[3]) - stateCount[2] / 2.0f : 0.0/0.0;
}

bool Detetcor::haveMultiplyConfirmedCenters() {
    return false;
}

int Detetcor::getRowSkip() {
    int max = finderCenters.size();
    if (max <= 1) {
        return 0;
    }
    FinderPoint& firstCenter = finderCenters[0];
    FinderPoint& secondCenter = finderCenters[1];
    hasSkipped = true;
    return (int)(abs(firstCenter.get_x() - secondCenter.get_x()) - abs(firstCenter.get_y()
                                                                          - secondCenter.get_y()))/2;
}
