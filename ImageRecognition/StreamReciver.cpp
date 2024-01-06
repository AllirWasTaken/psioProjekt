#include "StreamReciver.h"
#include <stdexcept>

CameraStream::CameraStream() {
    const std::string videoStreamAddress = "http://192.168.1.100:8080/videofeed?something.mjpeg";
    //open the video stream and make sure it's opened
    if(!handle.open(videoStreamAddress)) {
        throw std::runtime_error("Stream could not open from given url");
    }
}


void CameraStream::DisplayFrame(Image &image) {
    image.ConvertImageToStream(raw.data);
    cv::imshow("Camera", raw);
    cv::waitKey(1);
}

void CameraStream::GetFrame(Image &image) {
    handle.read(raw);
    image.ConvertStreamToImage(raw.data);
}

void CameraStream::GetEdgedImage(Image &image, int threshold1, int threshold2) {
    cv::Canny(raw,edged,threshold1,threshold2);
    image.ConvertStreamToImage(edged.data,1);
    image.ConvertCannyDetectionToNormal();
}