#pragma once

#include<cstdint>
#include<vector>
#include "TaskStack.h"



#define EDGE_COLOR 150
#define BACKGROUND_FILL 125

class Pixel{
    public:
    uint8_t r,g,b;
    Pixel& operator=(uint8_t a);
};


class Image{
    private:
    int imageX,imageY;
    std::vector<std::vector<Pixel>> imageData;
    std::vector<std::vector<Pixel>> tempImageData;

    int MaxMeanValueDiffAroundPixel(int y,int x);
    int BlobingMaxValueAround(int x,int y, int currentVal);
    void AddBorderingToStack(Point2& pix,TaskStack& stack);
    int GetWhitePixelsAround(int x, int y);

    public:
    Image(int x=0,int y=0);
    void ConvertStreamToImage(const unsigned char* stream,int colorChanelCount=3);
    void ConvertImageToStream(unsigned char* stream);
    void CutImage(Image& dest, int beginX=-1,int beginY=-1);
    void FitIntoImage(Image& src,int beginX=-1,int beginY=-1);
    void EdgeDetection(int tolerance=0);
    void BlobEdges(int blobing=2);
    void FilterOutNoise(TaskStack &taskStack,int minimumPixelThreshold=0);
    void Antialiasing(int iterations);
    void LoadBmpImage(const char* path);
    void SaveBmpImage(const char* path);
    void ConvertCannyDetectionToNormal();
    int Height();
    int Width();
    void DrawSquare(int x,int y,int width,int height);
    std::vector<Pixel>& operator[](int index);
    Image& operator=(Image& image);

};