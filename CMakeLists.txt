cmake_minimum_required(VERSION 3.3)
project(QRcodeCPP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        Decoder/Decoder.cpp
        Decoder/Decoder.hpp
        Detector/Finder.cpp
        Detector/Finder.hpp
        Detector/FinderPoint.cpp
        Detector/FinderPoint.hpp
        Detector/FinderResult.cpp
        Detector/FinderResult.hpp
    main.cpp Detector/Detector.cpp Detector/Detector.h Version.cpp Version.h Detector/DetectorResult.cpp Detector/DetectorResult.h Decoder/FormatInfo.cpp Decoder/FormatInfo.h BitMatrix.cpp BitMatrix.h Decoder/DataBlock.cpp Decoder/DataBlock.h QRcodeReader.cpp QRcodeReader.h Decoder/DecoderResult.cpp Decoder/DecoderResult.h Binarizer.cpp Binarizer.h)

add_executable(QRcodeCPP ${SOURCE_FILES})

find_package(OpenCV REQUIRED core highgui imgproc)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(QRcodeCPP ${OpenCV_LIBS})