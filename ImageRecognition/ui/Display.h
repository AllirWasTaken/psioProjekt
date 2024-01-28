#pragma once

#include <chrono>
#include <string>

#include <opencv2/core/mat.hpp>

class Display
{
public:
    Display(std::string windowName);
    ~Display();

    Display(const Display& other) = delete;
    Display(Display&& other) = delete;

    Display& operator=(const Display& other) = delete;
    Display& operator=(Display&& other) = delete;

    void setFrame(cv::Mat frame);
    void displayCurrentFrame() const;
    void wait(const std::chrono::milliseconds& waitTime) const;

private:
    std::string name;
    cv::Mat currentFrame;
};