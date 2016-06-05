//
// Created by 范志方 on 16/6/5.
//

#ifndef QRCODECPP_DECODERRESULT_H
#define QRCODECPP_DECODERRESULT_H
#include <vector>
#include <string>

class DecoderResult {
    int version;
    int ecLevel;
    std::vector<std::vector<char> > codeWords;
    std::vector<char> resultBytes;
    std::string resultText;

public:

    DecoderResult(int version, int ecLevel, std::vector<std::vector<char>> codeWords,
                  std::vector<char> resultBytes, std::string resultText) : version(version),
                                                                                         ecLevel(ecLevel),
                                                                                         codeWords(codeWords),
                                                                                         resultBytes(resultBytes),
                                                                                         resultText(resultText) { }

    int getVersion() const {
        return version;
    }

    int getEcLevel() const {
        return ecLevel;
    }

    const std::vector<std::vector<char>> &getCodeWords() const {
        return codeWords;
    }

    const std::vector<char> &getResultBytes() const {
        return resultBytes;
    }

    const std::string getResultText() const {
        return resultText;

    }
};


#endif //QRCODECPP_DECODERRESULT_H
