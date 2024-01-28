#pragma once

#include <future>
#include <thread>

#include <opencv2/core/mat.hpp>

#include <common/async/ThreadPool.h>

class ObjectDetection {
public:
    ObjectDetection(ThreadPool& threadPool);

    void setFrame(cv::Mat frame);
    std::future<cv::Mat> processFrame();

private:
    cv::Mat currentFrame;
    ThreadPool& threadPool;
};