//
// Created by 范志方 on 16/5/28.
//

#include <stdlib.h>
#include "BitMatrix.h"
using namespace std;
int BitMatrix::getDimension() const {
    return dimension;
}

bool BitMatrix::get(int x, int y) {
    return bits[y * dimension + x];
}

void BitMatrix::XOR(vector<bool> &mask) {
    if (mask.size() != dimension*dimension) {
        printf("Bad Mask\n");
        exit(1);
    }
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            bits[i*dimension + j] = (int)bits[i*dimension + j] ^ (int)mask[i*dimension + j];
        }
    }
}

void BitMatrix::display() {
    for (int iy = 0; iy < dimension; ++iy) {
        int jx = 0;
        for (; jx < dimension; ++jx) {
            if (bits[iy*dimension + jx]) {
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
}


BitMatrix::BitMatrix(int dimension)  : dimension(dimension) {
    bits = vector<bool>(dimension*dimension, false);
}

void BitMatrix::setRegion(int left, int top, int width, int height) {
    if(top >= 0 && left >= 0) {
        if(height >= 1 && width >= 1) {
            int right = left + width;
            int bottom = top + height;
            if(bottom <= dimension && right <= dimension) {
                for(int y = top; y < bottom; ++y) {
                    int offset = y * dimension;

                    for(int x = left; x < right; ++x) {
                        bits[offset + x] = true;
                    }
                }
            }
        }
    }
}

