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
    Config *config;
    std::vector<SampledData> apple,orange,kiwi,avocado,citrus;
    static SampledData SampleObjectData(Image&image, Object& object);
    static SampledData SampleImageData(Image&image);
    static bool DoesThisExist(std::string name);
    static float MatchObject(std::vector<SampledData> data, SampledData object);

public:
    ObjectClassifier(Config *configF);
    void AnalyzeAndCategorize(Image& image, Object& object);
    void AnalyzeSavedSamples(const char* path);
};
