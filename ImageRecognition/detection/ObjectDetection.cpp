#include "ObjectDetection.h"

#include <climits>
#include <memory>
#include <vector>

#include <opencv2/imgproc.hpp>

#include <common/Config.h>

ObjectDetection::ObjectDetection(ThreadPool& threadPool)
    : threadPool(threadPool)
{
}

void ObjectDetection::setFrame(cv::Mat frame)
{
    currentFrame = std::move(frame);
}

std::future<cv::Mat> ObjectDetection::processFrame()
{
    auto promise = std::make_shared<std::promise<cv::Mat>>();
    auto future = promise->get_future();

    threadPool.schedule([this, promise = std::move(promise)](
                            std::stop_token stopToken) mutable {
        try {
            if (stopToken.stop_requested())
                return;
            cv::Mat edges;
            auto screenRect = cv::Rect(
                0, 0, currentFrame.size().width, currentFrame.size().height);
            if (stopToken.stop_requested())
                return;
            cv::Canny(currentFrame, edges, 50, 150, 3, true);
            if (stopToken.stop_requested())
                return;
            cv::Mat kernel
                = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
            cv::Mat dilatedEdges;
            cv::dilate(edges, dilatedEdges, kernel);
            if (stopToken.stop_requested())
                return;
            cv::rectangle(dilatedEdges, screenRect, cv::Scalar(255, 255, 255));
            if (stopToken.stop_requested())
                return;
            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;
            cv::findContours(dilatedEdges, contours, hierarchy, cv::RETR_TREE,
                cv::CHAIN_APPROX_SIMPLE);
            if (stopToken.stop_requested())
                return;
            cv::Mat objectMask = cv::Mat::zeros(dilatedEdges.size(), CV_8UC3);
            cv::Mat boundingBoxes
                = cv::Mat::zeros(dilatedEdges.size(), CV_8UC3);
            for (int i = 0; i < contours.size(); ++i) {
                cv::Point topLeft(INT_MAX, INT_MAX);
                cv::Point bottomRight(INT_MIN, INT_MIN);
                for (const auto& point : contours[i]) {
                    topLeft.x = std::min(topLeft.x, point.x);
                    bottomRight.x = std::max(bottomRight.x, point.x);

                    topLeft.y = std::min(topLeft.y, point.y);
                    bottomRight.y = std::max(bottomRight.y, point.y);
                }
                cv::Rect objectRect(topLeft, bottomRight);
                if (stopToken.stop_requested())
                    return;
                if (objectRect.area() < screenRect.area() * 0.3
                    || objectRect.area() >= screenRect.area() * 0.95)
                    continue;
                // cv::calcHist();
                cv::drawContours(objectMask, contours, i,
                    cv::Scalar(255, 255, 255), cv::FILLED);
                cv::rectangle(boundingBoxes, objectRect, cv::Scalar(0, 255, 0));
            }
            cv::Mat result;
            cv::bitwise_or(objectMask, boundingBoxes, result);
            promise->set_value(dilatedEdges);
        } catch (...) {
            promise->set_exception(std::current_exception());
        }
    });
    return future;
}