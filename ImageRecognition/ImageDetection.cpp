#include "ImageDetection.h"

#include <chrono>
#include <iostream>

#include <common/Config.h>
#include <common/Exceptions.h>

using namespace std::chrono_literals;

namespace {

constexpr ImageDetection::Clock::duration framePeriod()
{
    constexpr auto period
        = std::chrono::duration<int, std::ratio<1, config::TargetFps>>(1);
    return std::chrono::duration_cast<ImageDetection::Clock::duration>(period);
}

std::string getCameraUrl()
{
    if(auto cameraUrl = std::getenv("CAMERA_URL"); cameraUrl)
        return cameraUrl;
    else
        std::cerr << "Please set environmental variable CAMERA_URL and restart program." << std::endl;
        throw ImageRecognitionError("Environmental variable CAMERA_URL not set");
}

} // namespace

ImageDetection::ImageDetection(ThreadPool& threadPool)
    : camera(getCameraUrl(), threadPool)
    , detection(threadPool)
    , display("Image detection")
    , threadPool(threadPool)
{
}

ImageDetection::~ImageDetection() = default;

int ImageDetection::run(std::stop_token stopToken)
{
    while (!stopToken.stop_requested()) {
        auto deadline = Clock::now() + framePeriod();
        auto frameFuture = camera.requestNextFrame();
        display.displayCurrentFrame();

        processFrame(stopToken, frameFuture, deadline);

        if (stopToken.stop_requested())
            break;
        display.wait(std::chrono::duration_cast<std::chrono::milliseconds>(
            deadline - Clock::now()));
    }
    return 0;
}

bool ImageDetection::processFrame(std::stop_token stopToken,
    std::future<cv::Mat>& frameFuture, const Clock::time_point& deadline)
try {
    if (stopToken.stop_requested()
        || frameFuture.wait_until(deadline) == std::future_status::timeout)
        return false;
    detection.setFrame(frameFuture.get());
    auto processedFrameFuture = detection.processFrame();
    if (stopToken.stop_requested()
        || processedFrameFuture.wait_until(deadline)
            == std::future_status::timeout)
        return false;
    display.setFrame(processedFrameFuture.get().clone());
    return true;
} catch (...) {
    return false;
}