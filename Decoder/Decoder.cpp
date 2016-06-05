//
//  Decoder.cpp
//  QRcodeCPP
//
//  Created by 范志方 on 16/5/24.
//  Copyright © 2016年 范志方. All rights reserved.
//

#include <string>
#include "Decoder.hpp"
#include "FormatInfo.h"
#include "DataBlock.h"
#include "lib/rs.hpp"
#include "Version.h"
#include "DecoderResult.h"

Decoder::Decoder(DetectorResult &detectorResult) {
    resultPoints = detectorResult.getResultPoints();
    bits = detectorResult.getBits();
}

DecoderResult Decoder::decode() {
    Version  version(bits.getDimension());
    FormatInfo formatInfo = FormatInfo(bits);
    releaseMask(formatInfo.getMaskPattern());
    vector<char> codewords = readCodeWords();
//    for (int i = 0; i < codewords.size(); ++i) {
//        printf("%d ", codewords[i]);
//    }
//    printf("\n");
    vector<DataBlock> dataBlocks = DataBlock::getDataBlocks(codewords, version, formatInfo.getErrorCorrectionLevel());
    int totalBytes = 0;

    vector<vector<char> > blocksOfwords;
    for (int i = 0; i < dataBlocks.size(); i++) {
        totalBytes += dataBlocks[i].getNumDataCodewords();
        vector<char> codewordInBlock = dataBlocks[i].getCodewords();
        blocksOfwords.push_back(codewordInBlock);
//        printf("Block %d:(size: %d)",i, codewordInBlock.size());
//        for (int i = 0; i < codewordInBlock.size(); ++i) {
//            printf("%d ", codewordInBlock[i]);
//        }
//        printf("\n");
    }
    int resultOffset = 0;
    vector<char> resultBytes(totalBytes);
    vector<char> correctedData;
    for (int j = 0; j < dataBlocks.size(); j++) {
        DataBlock dataBlock = dataBlocks[j];
        vector<char> codewordBytes = dataBlock.getCodewords();
        int numDataCodewords = dataBlock.getNumDataCodewords();
        correctedData = correctErrors(codewordBytes, numDataCodewords);
        for (int i = 0; i < numDataCodewords; i++) {
            resultBytes[resultOffset++] = correctedData[i];
        }
    }
//
    string resultText = decodeFinal(resultBytes);
    DecoderResult decoderResult = DecoderResult(version.getVersionNumber(), formatInfo.getErrorCorrectionLevel(),
                                                blocksOfwords, resultBytes, resultText);
    return decoderResult;
}

void Decoder::releaseMask(int maskPattern) {
//    printf("MaskPattern: %d\n",maskPattern);
    int dimension = bits.getDimension();
    vector<bool> mask(dimension * dimension);
    if (maskPattern == 0) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = ((i + j) % 2 == 0);
            }
        }
    }
    if (maskPattern == 1) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = (i % 2 == 0);
            }
        }
    }
    if (maskPattern == 2) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = (j % 3 == 0);
            }
        }
    }
    if (maskPattern == 3) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = ((i + j) % 3 == 0);
            }
        }
    }
    if (maskPattern == 4) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = (((i / 2) + (j / 3)) % 2 == 0);
            }
        }
    }
    if (maskPattern == 5) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = ((i*j) % 2 + (i*j) % 3 == 0);
            }
        }
    }
    if (maskPattern == 6) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = (((i*j) % 2 +(i*j) % 3)%2 == 0);
            }
        }
    }

    if (maskPattern == 7) {
        for (int i = 0; i < dimension; ++i) {
            for (int j = 0; j < dimension; ++j) {
                mask[i * dimension + j] = (((i*j) % 3 + (i+j) % 2) % 2 == 0);
            }
        }
    }
    bits.XOR(mask);
    BitMatrix test = BitMatrix(mask, dimension);

//    printf("\n");
//    bits.display();

}

vector<char> Decoder::readCodeWords() {
    BitMatrix functionPattern = buildFunctionPattern();
//    functionPattern.display();
    int dimension = bits.getDimension();
    bool readingUp = true;
    vector<char> result;//getTotalCodewords()
    int resultOffset = 0;
    int currentByte = 0;
    int bitsRead = 0;
    // Read columns in pairs, from right to left
    for (int x = dimension - 1; x > 0; x -= 2) {
        if (x == 6) {
            // Skip whole column with vertical alignment pattern;
            // saves time and makes the other code proceed more cleanly
            x--;
        }
        // Read alternatingly from bottom to top then top to bottom
        for (int counter = 0; counter < dimension; counter++) {
            int y = readingUp ? dimension - 1 - counter : counter;
            for (int col = 0; col < 2; col++) {
                // Ignore bits covered by the function pattern
                if (!functionPattern.get(x - col, y)) {
                    // Read a bit
                    bitsRead++;
                    currentByte <<= 1;
                    if (bits.get(x - col, y)) {
                        currentByte |= 1;
                    }
                    // If we've made a whole byte, save it off
                    if (bitsRead == 8) {
                        result.push_back((char)currentByte);
                        bitsRead = 0;
                        currentByte = 0;
                    }
                }
            }
        }
        readingUp = !readingUp; // switch directions
    }

//    if (resultOffset != version->getTotalCodewords()) {
//        throw ReaderException("Did not read all codewords");
//    }
    return result;
}

BitMatrix Decoder::buildFunctionPattern() {
    int dimension = bits.getDimension();
    BitMatrix result = BitMatrix(dimension);
    result.setRegion(0, 0, 9, 9);
    result.setRegion(dimension - 8, 0, 8, 9);
    result.setRegion(0, dimension - 8, 9, 8);
//    printf("%d\n", resultPoints.size());
    if (resultPoints.size() == 4) result.setRegion(dimension - 9, dimension - 9, 5, 5);
    result.setRegion(6, 9, 1, dimension - 17);
    result.setRegion(9, 6, dimension - 17, 1);
    return result;
}

vector<char> Decoder::correctErrors(vector<char> codewordBytes, int numDataCodewords) {
    int numCodewords = codewordBytes.size();
//    vector<int> codewordInts(numCodewords);
//    for (int i = 0; i < numCodewords; i++) {
//        codewordInts[i] = codewordBytes[i] & 0xff;
//    }
    int numECCodewords = numCodewords - numDataCodewords;
    char encoded[numCodewords];
    for (int i = 0; i < numCodewords; i++) {
        encoded[i] = codewordBytes[i];
    }
    char repaired[numDataCodewords];
//    printf("rs: %d %d\n",numDataCodewords, numECCodewords);
    RS::ReedSolomon rs(numDataCodewords, numECCodewords);
    rs.Decode(encoded,repaired);
    vector<char> result;
//    printf("total:%d data:%d\n", numCodewords, numDataCodewords);
    for (int i = 0; i < numCodewords; i++) {
        result.push_back(repaired[i]);
    }
    return result;
}

string Decoder::decodeFinal(vector<char> resultBytes) {
    string result;
    int mode = resultBytes[0] >> 4;
    char size = (resultBytes[0] << 4) | (resultBytes[1] >> 4 & 0x0f);
    if (mode != 4 || size < 0) {
        printf("This mode is not supported\n");
        exit(1);
    }
    for (int i = 0; i < size; ++i) {
        char tmp = (resultBytes[i+1] << 4 & 0xf0) | (resultBytes[i+2] >> 4 & 0x0f);
        result.push_back(tmp);
    }

//    printf("Mode: %d, Length: %d, Result: %s\n", mode, size, result.c_str());
    return result;
}
