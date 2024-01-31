#include "ObjectClassifier.h"
#include <string>
#include <cmath>

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
    for(int i=0;i<255;i++){
        result.histoGraph.r[i]=0;
        result.histoGraph.g[i]=0;
        result.histoGraph.b[i]=0;
    }

    for(int y=0;y<image.Height();y++){
        for(int x=0;x<image.Width();x++){
            r+=image[y][x].r;
            g+=image[y][x].g;
            b+=image[y][x].b;

            result.histoGraph.r[image[y][x].r]++;
            result.histoGraph.g[image[y][x].g]++;
            result.histoGraph.b[image[y][x].b]++;
        }
    }
    r/=image.Height()*image.Width();
    g/=image.Height()*image.Width();
    b/=image.Height()*image.Width();
    result.average.r=r;
    result.average.b=b;
    result.average.g=g;

    float mr=0,mg=0,mb=0;

    for(int i=0;i<255;i++){
        if(result.histoGraph.r[i]>mr){
            mr=result.histoGraph.r[i];
        }if(result.histoGraph.g[i]>mg){
            mg=result.histoGraph.g[i];
        }if(result.histoGraph.b[i]>mb){
            mb=result.histoGraph.b[i];
        }
    }
    for(int i=0;i<255;i++) {
        result.histoGraph.r[i]/=mr;
        result.histoGraph.g[i]/=mg;
        result.histoGraph.b[i]/=mb;
    }


        return result;
}

SampledData ObjectClassifier::SampleObjectData(Image &image, Object &object) {
    int step=100/100;
    int size=object.bodyPixels.size();
    SampledData result;
    size_t r,g,b;
    r=0;
    g=0;
    b=0;

    for(int i=0;i<255;i++){
        result.histoGraph.r[i]=0;
        result.histoGraph.g[i]=0;
        result.histoGraph.b[i]=0;
    }
    Point2 point;

    int x,y;

    for(int i=0;i<size;i+=step){
        point=object.bodyPixels[i];
        x=point.x;
        y=point.y;

        r+=image[y][x].r;
        g+=image[y][x].g;
        b+=image[y][x].b;

        result.histoGraph.r[image[y][x].r]++;
        result.histoGraph.g[image[y][x].g]++;
        result.histoGraph.b[image[y][x].b]++;
    }

    r/=size/step;
    g/=size/step;
    b/=size/step;
    result.average.r=r;
    result.average.b=b;
    result.average.g=g;

    float mr=0,mg=0,mb=0;

    for(int i=0;i<255;i++){
        if(result.histoGraph.r[i]>mr){
            mr=result.histoGraph.r[i];
        }if(result.histoGraph.g[i]>mg){
            mg=result.histoGraph.g[i];
        }if(result.histoGraph.b[i]>mb){
            mb=result.histoGraph.b[i];
        }
    }
    for(int i=0;i<255;i++) {
        result.histoGraph.r[i]/=mr;
        result.histoGraph.g[i]/=mg;
        result.histoGraph.b[i]/=mb;
    }

    return result;
}

float my_abs(float x){
    if(x<0)return x*-1;
    else return x;
}

float ObjectClassifier::MatchObject(std::vector<SampledData> data, SampledData object) {
    float max=0;
    std::vector<float> diffR,diffB,diffG;
    float match=0;
    diffR.resize(255);
    diffG.resize(255);
    diffB.resize(255);
    float temp;

    for(int i=0;i<data.size();i++){


        for(int j=0;j<255;j++){
            diffR[j]=my_abs(data[i].histoGraph.r[j]-object.histoGraph.r[j]);
            diffG[j]=my_abs(data[i].histoGraph.g[j]-object.histoGraph.g[j]);
            diffB[j]=my_abs(data[i].histoGraph.b[j]-object.histoGraph.b[j]);
        }

        match =1;
        for(int j=0;j<255;j++){
            match*=(255-diffR[j])/(float)255;
            match*=(255-diffG[j])/(float)255;
            match*=(255-diffB[j])/(float)255;
        }


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

HistoGraph::HistoGraph() {
    r.resize(255);
    g.resize(255);
    b.resize(255);
}