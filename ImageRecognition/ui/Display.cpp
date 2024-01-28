#include "Display.h"

#include <opencv2/highgui.hpp>

#include "Exceptions.h"

using namespace std::chrono_literals;

Display::Display(std::string windowName)
    : name(std::move(windowName))
    , currentFrame(720, 720, CV_8UC3, cv::Scalar(0, 0, 0))
{
    cv::imshow(name, currentFrame);
}

Display::~Display()
{
    cv::destroyWindow(name);
}

void Display::setFrame(cv::Mat frame)
{
    currentFrame = std::move(frame);
}

void Display::displayCurrentFrame() const
{
    cv::imshow(name, currentFrame);
}

void Display::wait(const std::chrono::milliseconds& waitTime) const
{
    if(waitTime >= 1ms)
        cv::waitKey(waitTime.count());
    else
        cv::waitKey(1);
}