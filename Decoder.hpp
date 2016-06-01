//
//  Decoder.hpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#ifndef Decoder_hpp
#define Decoder_hpp

#include <stdio.h>
#include "DetectorResult.h"
#include "FormatInfo.h"

class Decoder {
private:
    vector<FinderPoint> resultPoints;
    BitMatrix bits;
    BitMatrix buildFunctionPattern();
    vector<char> readCodeWords();
    vector<char> correctErrors(vector<char> codewordBytes, int numDataCodewords);
    string decodeFinal(vector<char> resultBytes);
public:

    Decoder(DetectorResult& detectorResult);
    void releaseMask(int maskPattern);
    void decode();
};

#endif /* Decoder_hpp */
