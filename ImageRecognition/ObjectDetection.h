#pragma once
#include "Image.h"

class Object{
    public:
    //std::vector<Point2> edgePixels;
    std::vector<Point2> bodyPixels;
    void CalculateVariables();
    Point2 maxPos;
    Point2 minPos;
    Point2 pos;
    int height,width;
    int type=0;

};


class ObjectDetection{
    private:
    TaskStack stack;
    int idCount;
    std::vector<Object> objects;
    void DeepSearchSetValue(Image& image,TaskStack& stack,int find,int set);
    void DeepSearchSetValueAndAddToStack(Image& image, TaskStack& stack, TaskStack& resultStack,int find,int set);



    public:
    void DetectObjects(Image& image,int filterThreshold);
    void DrawObjects(Image& image);
    //void MakeNewEdge(Image& image,int thickness);
    void CalculateObjectsVariables();
    void OffestObjects(int x,int y);
    Object& operator[](int index);
    int Size();
};