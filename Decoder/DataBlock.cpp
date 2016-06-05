//
// Created by 范志方 on 16/5/31.
//

#include <stdlib.h>
#include "DataBlock.h"
#include "Version.h"

vector<DataBlock> DataBlock::getDataBlocks(vector<char> &codewords, Version &version, int ecLevel) {
//    shoudl encapsulate it to Version
    int totalBlocks = version.getNumBlocks(ecLevel);
    int ecCodewordsNum = version.getNumEcCodewords(ecLevel);
    int DataCodewordsNum = codewords.size() - ecCodewordsNum;
    int DataCodewordsInBlock = DataCodewordsNum / totalBlocks;
    int ECcodewordsInBlock = ecCodewordsNum / totalBlocks;
//    printf("Version = %d, ecLevel = %d\n", version.getVersionNumber(), ecLevel);
//    if (version == 4 && ecLevel == 2) {
//        totalBlocks = 4;
//        DataCodewordsNum = 36;
//        DataCodewordsInBlock = DataCodewordsNum / totalBlocks;
//        ECcodewordsInBlock = 16;
//    }
//    else {
//        printf("This version is not supported\n");
//        exit(1);
//    }
    vector<DataBlock> result(totalBlocks);
    for (int blockIndex = 0; blockIndex < totalBlocks; ++blockIndex) {
        result[blockIndex].numDataCodewords_ = DataCodewordsInBlock;
        for (int i = 0; i < DataCodewordsInBlock; ++i) {
            result[blockIndex].codewords_.push_back(codewords[i*totalBlocks + blockIndex]);
        }
        for (int j = 0; j < ECcodewordsInBlock; ++j) {
            result[blockIndex].codewords_.push_back(codewords[j*totalBlocks + blockIndex + DataCodewordsNum]);
        }
    }
    return result;
}
