#pragma once
#include "Image.h"

class Object{
    public:
    std::vector<Point2> edgePixels;
    std::vector<Point2> bodyPixels;
    void CalculateVariables();
    Point2 maxPos;
    Point2 minPos;
    Point2 pos;
    int height,width;

};


class ObjectDetection{
    private:
    int idCount;
    std::vector<Object> objects;
};