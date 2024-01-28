#pragma once

#include <future>
#include <mutex>
#include <string>
#include <thread>

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include <common/async/ThreadPool.h>

class CameraConnection
{
public:
    CameraConnection(std::string url, ThreadPool& threadPool);

    std::future<cv::Mat> requestNextFrame();

private:
    std::string url;
    cv::VideoCapture camera;
    ThreadPool& threadPool;
};