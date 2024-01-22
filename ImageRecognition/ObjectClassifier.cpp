#include "ObjectClassifier.h"
#include <string>


void ObjectClassifier::AnalyzeSavedSamples(const char *path) {
    std::string Path=path;
    std::string orangePath=Path+"orange/1.bmp";

    Image orangeImg;
    orangeImg.LoadBmpImage(orangePath.data());
    orange= SampleImageData(orangeImg);
    Image citrusImg;
    std::string citrusPath=Path+"citrus/1.bmp";
    citrusImg.LoadBmpImage(citrusPath.data());
    citrus= SampleImageData(citrusImg);
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

void ObjectClassifier::AnalyzeAndCategorize(Image &image, Object &object) {
    SampledData current;
    current= SampleObjectData(image,object);
    float diffR,diffB,diffG;


    //orange
    diffR=255-abs(orange.average.r-current.average.r);
    diffR/=255;
    diffG=255-abs(orange.average.g-current.average.g);
    diffG/=255;
    diffB=255-abs(orange.average.b-current.average.b);
    diffB/=255;
    float orangeMatch=diffR*diffB*diffG;

    //citrus
    diffR=255-abs(citrus.average.r-current.average.r);
    diffR/=255;
    diffG=255-abs(citrus.average.g-current.average.g);
    diffG/=255;
    diffB=255-abs(citrus.average.b-current.average.b);
    diffB/=255;
    float citrusMatch=diffR*diffB*diffG;
    if(citrusMatch>0.76){
        object.type=4;
    }
    else if(orangeMatch>0.76){
        object.type=2;
    }
    else{
        object.type=0;
    }

}