#include <opencv2/opencv.hpp>
#include <iostream>

int main(int, char**) {
    cv::VideoCapture vcap;
    cv::Mat image;

    const std::string videoStreamAddress = "http://192.168.1.100:8080/videofeed?something.mjpeg";
    //open the video stream and make sure it's opened
    if(!vcap.open(videoStreamAddress)) {
        std::cerr << "Error opening video stream or file" << std::endl;
        return -1;
    }

    for(;;) {
        vcap.read(image);
        cv::imshow("Output Window", image);
        cv::waitKey(1);
    }

}