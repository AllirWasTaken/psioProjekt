#include "StreamReciver.h"
#include <cstdlib>
#include <stdexcept>

CameraStream::CameraStream() {
    const std::string videoStreamAddress = []
    {
        const std::string prefix = "http://";
        const std::string suffix = "/videofeed?something.mjpeg";

        if (const char* customUrl = std::getenv("IMG_HOST"); customUrl != nullptr)
            return prefix + std::string{customUrl} + suffix;

        const std::string defaultUrl = "192.168.1.100:8080";
        return prefix + defaultUrl + suffix;
    }();
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