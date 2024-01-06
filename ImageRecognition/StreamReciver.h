#pragma once
#include <opencv2/opencv.hpp>
#include "Image.h"
class CameraStream {
public:
    cv::VideoCapture handle;
    CameraStream();
    void GetFrame(Image& image);
    void DisplayFrame(Image& image);
    void GetEdgedImage(Image& image, int threshold1, int threshold2);
private:
    cv::Mat raw;
    cv::Mat edged;
};
