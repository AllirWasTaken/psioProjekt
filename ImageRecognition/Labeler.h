#pragma once
#include "Image.h"
#include "ObjectDetection.h"

class Labeler {
private:
    Image apple,banana, orange, citrus, object;
public:
    Labeler(const char* path);
    void Label(Image& image,Object& objectVar);

};