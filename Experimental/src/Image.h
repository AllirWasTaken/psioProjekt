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
    TaskStack taskStack;

    int MaxMeanValueDiffAroundPixel(int y,int x);
    int BlobingMaxValueAround(int x,int y, int currentVal);
    void AddBorderingToStack(Point2& pix,TaskStack& stack);
    int GetWhitePixelsAround(int x, int y);

    public:
    Image(int x,int y);
    void ConvertStreamToImage(std::vector<uint8_t>& stream);
    void ConvertImageToStream(std::vector<uint8_t>& stream);
    void CutImage(Image& dest);
    void FitIntoImage(Image& src);
    void EdgeDetection(int tolerance=0);
    void BlobEdges(int blobing=2);
    void FilterOutNoise(int minimumPixelThreshold=0); 
    void Antialiasing(int iterations);
    int Height();
    int Width();
    void DrawSquare(int x,int y,int size);
    std::vector<Pixel>& operator[](int index);

};