cmake_minimum_required(VERSION 3.3)
project(QRcodeCPP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Decoder.cpp
    Decoder.hpp
    Finder.cpp
    Finder.hpp
    FinderPoint.cpp
    FinderPoint.hpp
    FinderResult.cpp
    FinderResult.hpp
    main.cpp Detector.cpp Detector.h Version.cpp Version.h)

add_executable(QRcodeCPP ${SOURCE_FILES})

find_package(OpenCV REQUIRED core highgui imgproc)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(QRcodeCPP ${OpenCV_LIBS})