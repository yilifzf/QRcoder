//
// Created by 范志方 on 16/6/5.
//

#include "QRcodeReader.h"
#include "Binarizer.h"


QRcodeReader::QRcodeReader(Mat &rgbImg, bool more) : rgbImg(rgbImg), more(more){
    cvtColor(rgbImg, img, CV_BGR2GRAY);
//    double minGray,maxGray;
//    minMaxLoc(gray,&minGray,&maxGray);
////    equalizeHist(gray,gray);
////    printf("Lumination: min: %f, max:%f\n", minGray, maxGray);
//    threshold(gray, img, 150, 255, THRESH_BINARY);
}

void QRcodeReader::decode() {
    Binarizer binarizer(img);
    img = binarizer.getBlackMatrix();

    if (more) {
        imshow("original", rgbImg);
        imshow("test",img);
        waitKey(0);
        printf("**************************************************************\n");
        printf("Begin detection to find the three finder pattern centers:\n");
    }

    Finder finder = Finder(img);
    FinderResult fr = finder.find();
    if (more) {
        printf("\n");
        printf("Three finder pattern centers:\n");
        FinderPoint bL = fr.getBottomLeft();
        FinderPoint tL = fr.getTopLeft();
        FinderPoint tR = fr.getTopRight();
        printf("bottomLeft: (%f, %f)\n", bL.getX(), bL.getY());
        printf("topLeft: (%f, %f)\n", tL.getX(), tL.getY());
        printf("topRight: (%f, %f)\n", tR.getX(), tR.getY());
        Point2f p1 = Point2f(bL.getX(), bL.getY());
        circle(rgbImg, p1, 3, Scalar(0,255,0));
        Point2f p2 = Point2f(tL.getX(), tL.getY());
        circle(rgbImg, p2, 3, Scalar(0,255,0));
        Point2f p3 = Point2f(tR.getX(), tR.getY());
        circle(rgbImg, p3, 3, Scalar(0,255,0));
        imshow("original", rgbImg);
        waitKey(0);
    }

    Detector detector = Detector(img);
    DetectorResult detectorResult = detector.processFinderPatternInfo(fr);
    if (more) {
        vector<FinderPoint> patternPoints = detectorResult.getResultPoints();
        BitMatrix bits = detectorResult.getBits();
        printf("\n");
        printf("Module Size: %f\n", detectorResult.getModuleSize());
        printf("Dimension: %d\n", detectorResult.getDimension());
        printf("Alignment Pattern : (%f, %f)\n", patternPoints[3].getX(), patternPoints[3].getY());
        Point2f p4 = Point2f(patternPoints[3].getX(), patternPoints[3].getY());
        circle(rgbImg, p4, 3, Scalar(0,255,0));
        imshow("original", rgbImg);
        waitKey(0);

        printf("\n");
        printf("The bit matrix:\n");
        bits.display();
        printf("\nDetection Done!\n");
        printf("**************************************************************\n");
        waitKey(0);
    }


    Decoder decoder = Decoder(detectorResult);
    DecoderResult decoderResult = decoder.decode();
    if (more) {
        printf("Decode:\n");
        printf("version : %d\n", decoderResult.getVersion());
        printf("Error correct level : %d\n", decoderResult.getEcLevel());
        vector<char> resultBytes = decoderResult.getResultBytes();
        printf("Data bytes: ");
        for (int i = 0; i < resultBytes.size(); ++i) {
            printf("%d ",resultBytes[i]);
        }
        printf("\n");
        string result = decoderResult.getResultText();
        printf("%s\n", result.c_str());
        waitKey(0);
    }
    else {
        string result = decoderResult.getResultText();
        printf("%s\n", result.c_str());
    }
}
