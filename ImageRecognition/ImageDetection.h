#pragma once

#include <thread>

#include <opencv2/core/mat.hpp>

#include <common/async/ThreadPool.h>
#include <detection/ObjectDetection.h>
#include <network/CameraConnection.h>
#include <ui/Display.h>

class ImageDetection
{
public:
    using Clock = std::chrono::steady_clock;

    ImageDetection(ThreadPool& threadPool);
    ~ImageDetection();

    int run(std::stop_token stopToken);

private:
    bool processFrame(std::stop_token stopToken, std::future<cv::Mat>& frameFuture, const Clock::time_point& deadline);

    CameraConnection camera;
    ObjectDetection detection;
    Display display;

    ThreadPool& threadPool;
};