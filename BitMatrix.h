//
// Created by 范志方 on 16/5/28.
//

#ifndef QRCODECPP_BITMATRIX_H
#define QRCODECPP_BITMATRIX_H
#include <vector>
using namespace std;
class BitMatrix {
private:
    int dimension;
    vector<bool> bits;

public:
    int getDimension() const;
    bool get(int x, int y);
    void XOR(vector<bool>& mask);

    BitMatrix() {}

    BitMatrix(int dimension);
    BitMatrix(const vector<bool> &bits, int dim) : bits(bits), dimension(dim) {}
    friend class FormatInfo;
    friend class Decoder;
    void display();
    void setRegion(int left, int top, int width, int height);
};


#endif //QRCODECPP_BITMATRIX_H
