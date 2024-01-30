#include "ObjectClassifier.h"
#include <string>


bool ObjectClassifier::DoesThisExist(std::string name) {
    bool result=true;

    FILE *f = fopen(name.c_str(),"rb");
    if(!f){
        result=false;
    }
    else{
        fclose(f);
    }

    return result;
}

void ObjectClassifier::AnalyzeSavedSamples(const char *path) {

    std::string currentPath;
    //ORANGE
    {
        std::string Path = path;
        std::string cPath = Path + "orange/";
        for(int i=0;true;i++) {
            currentPath=cPath+std::to_string(i+1)+".bmp";
            if(!ObjectClassifier::DoesThisExist(currentPath)){
                break;
            }
            Image Img;
            Img.LoadBmpImage(currentPath.c_str());
            orange.push_back(SampleImageData(Img));
        }
    }
    //CITRUS
    {
        std::string Path = path;
        std::string cPath = Path + "citrus/";
        for(int i=0;true;i++) {
            currentPath=cPath+std::to_string(i+1)+".bmp";
            if(!ObjectClassifier::DoesThisExist(currentPath)){
                break;
            }
            Image Img;
            Img.LoadBmpImage(currentPath.c_str());
            citrus.push_back(SampleImageData(Img));
        }
    }
    //APPLE
    {
        std::string Path = path;
        std::string cPath = Path + "apple/";
        for(int i=0;true;i++) {
            currentPath=cPath+std::to_string(i+1)+".bmp";
            if(!ObjectClassifier::DoesThisExist(currentPath)){
                break;
            }
            Image Img;
            Img.LoadBmpImage(currentPath.c_str());
            apple.push_back(SampleImageData(Img));
        }
    }
    //KIWI
    {
        std::string Path = path;
        std::string cPath = Path + "kiwi/";
        for(int i=0;true;i++) {
            currentPath=cPath+std::to_string(i+1)+".bmp";
            if(!ObjectClassifier::DoesThisExist(currentPath)){
                break;
            }
            Image Img;
            Img.LoadBmpImage(currentPath.c_str());
            kiwi.push_back(SampleImageData(Img));
        }
    }
    //AVOCADO
    {
        std::string Path = path;
        std::string cPath = Path + "avocado/";
        for(int i=0;true;i++) {
            currentPath=cPath+std::to_string(i+1)+".bmp";
            if(!ObjectClassifier::DoesThisExist(currentPath)){
                break;
            }
            Image Img;
            Img.LoadBmpImage(currentPath.data());
            avocado.push_back(SampleImageData(Img));
        }
    }

}

SampledData ObjectClassifier::SampleImageData(Image &image) {
    SampledData result;
    size_t r,g,b;
    r=0;
    g=0;
    b=0;

    for(int y=0;y<image.Height();y++){
        for(int x=0;x<image.Width();x++){
            r+=image[y][x].r;
            g+=image[y][x].g;
            b+=image[y][x].b;
        }
    }
    r/=image.Height()*image.Width();
    g/=image.Height()*image.Width();
    b/=image.Height()*image.Width();
    result.average.r=r;
    result.average.b=b;
    result.average.g=g;
    return result;
}

SampledData ObjectClassifier::SampleObjectData(Image &image, Object &object) {
    int step=100/10;
    int size=object.bodyPixels.size();
    SampledData result;
    size_t r,g,b;
    r=0;
    g=0;
    b=0;
    Point2 point;

    int x,y;

    for(int i=0;i<size;i+=step){
        point=object.bodyPixels[i];
        x=point.x;
        y=point.y;

        r+=image[y][x].r;
        g+=image[y][x].g;
        b+=image[y][x].b;
    }

    r/=size/step;
    g/=size/step;
    b/=size/step;
    result.average.r=r;
    result.average.b=b;
    result.average.g=g;
    return result;
}

float ObjectClassifier::MatchObject(std::vector<SampledData> data, SampledData object) {
    float max=0;
    float diffR,diffB,diffG;
    for(int i=0;i<data.size();i++){
        diffR=255-(float)abs(data[i].average.r-object.average.r);
        diffR/=255;
        diffG=255-(float)abs(data[i].average.g-object.average.g);
        diffG/=255;
        diffB=255-(float)abs(data[i].average.b-object.average.b);
        diffB/=255;
        float match=diffR*diffB*diffG;
        if(match>max){
            max=match;
        }
    }
    return max;
}


void ObjectClassifier::AnalyzeAndCategorize(Image &image, Object &object) {
    SampledData current;
    current= SampleObjectData(image,object);
    float currentMatch;
    float bestMatch=0;
    FruitType bestType=OBJECT;


    //orange
    currentMatch=ObjectClassifier::MatchObject(orange,current);
    if(currentMatch>bestMatch){
        bestMatch=currentMatch;
        bestType=ORANGE;
    }
    //citrus
    currentMatch=ObjectClassifier::MatchObject(citrus,current);
    if(currentMatch>bestMatch){
        bestMatch=currentMatch;
        bestType=CITRUS;
    }
    //apple
    currentMatch=ObjectClassifier::MatchObject(apple,current);
    if(currentMatch>bestMatch){
        bestMatch=currentMatch;
        bestType=APPLE;
    }
    //kiwi
    currentMatch=ObjectClassifier::MatchObject(kiwi,current);
    if(currentMatch>bestMatch){
        bestMatch=currentMatch;
        bestType=KIWI;
    }
    //avocado
    currentMatch=ObjectClassifier::MatchObject(avocado,current);
    if(currentMatch>bestMatch){
        bestMatch=currentMatch;
        bestType=AVOCADO;
    }
    if((float)config->typeThreshold/100>bestMatch){
        bestType=OBJECT;
    }
    object.type=bestType;
}

ObjectClassifier::ObjectClassifier(Config *configF) {
    config=configF;
}