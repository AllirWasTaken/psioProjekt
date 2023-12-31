#include "Config.h"



void Config::SetDefault(){
    videoY=720;
    videoX=720;
    videoWorkY=560;
    videoWorkX=710;

    calibrationMode=0;
    debugMode=1;

    edgeDetectionThreshold=100;
    blobEdgesAmount=2;
    filterNoiseThreshold=5000;
    antialiasingIterations=2;

    end=std::chrono::high_resolution_clock::now();
    work=true;
}


void Config::MeasureFps(){
    int averageFramCount=10;
    start=end;
    end=std::chrono::high_resolution_clock::now();
    float frameTime=(end-start).count();
    float currentFps=1000000000/frameTime;
    fps*=averageFramCount-1;
    fps+=currentFps;
    fps/=averageFramCount;
}