#include <iostream>
#include <stdexcept>

#include "ImageRecognition.h"

int main(int argc, char** argv)
{
    try {
        ImageRecognition imageRecognition;
        return imageRecognition.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown fatal error" << std::endl;
    }
    return -1;
}