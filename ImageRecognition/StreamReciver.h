#pragma once
#include <opencv2/opencv.hpp>
#include "Image.h"
class CameraStream {
public:
    cv::VideoCapture handle;
    CameraStream();
    void GetFrame(Image& image);
    void DisplayFrame(Image& image);
private:
    cv::Mat conversionImage;
};
