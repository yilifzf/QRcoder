//
// Created by 范志方 on 16/5/28.
//

#include <stdlib.h>
#include "FormatInfo.h"

FormatInfo::FormatInfo(BitMatrix &bits) {
    int formatBits1 = 0;
    for (int i = 0; i < 6; ++i) {
        if (bits.get(8, i)) {
            formatBits1 |= (1 << i);
        }
    }
    if (bits.get(8, 7)) {
        formatBits1 |= (1 << 6);
    }
    if (bits.get(8, 8)) {
        formatBits1 |= (1 << 7);
    }
    if (bits.get(7, 8)) {
        formatBits1 |= (1 << 8);
    }
    for (int j = 5; j >= 0; j--) {
        if (bits.get(j, 8)) {
            formatBits1 |= (1 << (14 - j));
        }
    }
    printf("%X\n", formatBits1);
    //
    int dimension = bits.getDimension();
    int formatBits2 = 0;
    for (int j = dimension - 1; j > dimension - 9; j--) {
        if (bits.get(j, 8)) {
            formatBits2 |= (1 << (dimension - 1 - j));
        }
    }
    for (int i = dimension - 7; i < dimension; ++i) {
        if (bits.get(8, i)) {
            formatBits2 |= (1 << (i - dimension + 15));
        }
    }
    printf("%X\n", formatBits2);
    if (formatBits1 != formatBits2) {
        printf("bad format\n");

    }
    int data = formatBits1 >> 10;
    ErrorCorrectionLevel = ((data >> 3) ^ 2) ^ 1;
    MaskPattern = (data & 7) ^ 5;
}

int FormatInfo::getErrorCorrectionLevel() const {
        return ErrorCorrectionLevel;
}

int FormatInfo::getMaskPattern() const {
    return MaskPattern;
}
