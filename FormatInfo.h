//
// Created by 范志方 on 16/5/28.
//

#ifndef QRCODECPP_FORMATINFO_H
#define QRCODECPP_FORMATINFO_H

#include <vector>
#include "BitMatrix.h"

using namespace std;
class FormatInfo {
private:
    int ErrorCorrectionLevel;
    int MaskPattern;
public:
    FormatInfo(BitMatrix& bits);

    int getErrorCorrectionLevel() const;

    int getMaskPattern() const;
};


#endif //QRCODECPP_FORMATINFO_H
