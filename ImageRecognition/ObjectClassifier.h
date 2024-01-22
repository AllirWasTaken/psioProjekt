#pragma once
#include "Config.h"
#include "Image.h"
#include "ObjectDetection.h"

class SampledData{
public:
    Pixel average;
};

class ObjectClassifier {
private:
    //static Config *config;
    SampledData apple,orange,banana,citrus;
    static SampledData SampleObjectData(Image&image, Object& object);
    static SampledData SampleImageData(Image&image);

public:
    //ObjectClassifier(Config *configF);
    void AnalyzeAndCategorize(Image& image, Object& object);
    void AnalyzeSavedSamples(const char* path);
};
