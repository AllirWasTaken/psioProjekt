#include "CameraConnection.h"

#include <memory>

#include "Exceptions.h"

CameraConnection::CameraConnection(std::string url, ThreadPool& threadPool)
    : url(std::move(url))
    , camera()
    , threadPool(threadPool)
{
}

std::future<cv::Mat> CameraConnection::requestNextFrame()
{
    auto promise = std::make_shared<std::promise<cv::Mat>>();
    auto future = promise->get_future();
    threadPool.schedule([this, promise](std::stop_token stopToken) mutable {
        try {
            if (!stopToken.stop_requested() && !camera.isOpened()
                && !camera.open(url))
                throw ConnectionFailedError("Failed to connect to camera");

            cv::Mat result;
            if (!stopToken.stop_requested() && !camera.read(result))
                throw ConnectionFailedError("Failed to download image");
            if (stopToken.stop_requested())
                return;
            promise->set_value(std::move(result));
        } catch (...) {
            promise->set_exception(std::current_exception());
        }
    });
    return future;
}