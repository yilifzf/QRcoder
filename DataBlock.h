//
// Created by 范志方 on 16/5/31.
//

#ifndef QRCODECPP_DATABLOCK_H
#define QRCODECPP_DATABLOCK_H

#include <vector>
#include "Version.h"

using namespace std;

class DataBlock {
    int numDataCodewords_;
    vector<char> codewords_;
public:
    static vector<DataBlock> getDataBlocks(vector<char> &codewords, Version &version, int ecLevel);

    int getNumDataCodewords() const {
        return numDataCodewords_;
    }

    const vector<char> &getCodewords() const {
        return codewords_;
    }
};


#endif //QRCODECPP_DATABLOCK_H
