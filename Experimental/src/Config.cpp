#include "Config.h"



void Config::SetDefault(){
    videoY=720;
    videoX=720;
    videoWorkY=700;
    videoWorkX=700;

    calibrationMode=0;
    debugMode=0;

    edgeDetectionThreshold=100;
    edgeRange=1;
    blobEdgesAmount=2;
    filterNoiseThreshold=2500;
    antialiasingIterations=0;

    ObjectNoiseThreshold=10000;
    detectObjects=1;

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

void Config::MeasureTime(){
    std::chrono::high_resolution_clock::time_point timeEnd;
    timeEnd=std::chrono::high_resolution_clock::now();
    time=(timeEnd-end).count()/1000000;
}