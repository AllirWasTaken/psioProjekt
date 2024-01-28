#pragma once

#include <memory>

class ImageRecognition
{
public:
    ImageRecognition();
    ~ImageRecognition();

    int run(int argc, char **argv);

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};