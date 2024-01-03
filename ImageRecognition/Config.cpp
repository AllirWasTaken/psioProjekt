#include "Config.h"



void Config::SetDefault(){
    videoY=720;
    videoX=1280;
    videoWorkY=700;
    videoWorkX=1200;

    calibrationMode=0;
    debugMode=0;

    edgeDetectionThreshold=40;
    blobEdgesAmount=0;
    filterNoiseThreshold=5000;
    antialiasingIterations=0;

    ObjectNoiseThreshold=20000;
    detectObjects=1;

    end=std::chrono::high_resolution_clock::now();
    work=true;
}


void Config::MeasureFps(){
    int averageFramCount=10;
    static int tick=1;
    if(tick){
        tick=0;
        start=end;
        end=std::chrono::high_resolution_clock::now();
        float frameTime=(end-start).count();
        float currentFps=1000000000/frameTime;
        fps*=averageFramCount-1;
        fps+=currentFps;
        fps/=averageFramCount;

    }
    else{
       auto temp=std::chrono::high_resolution_clock::now();
       time=(float)(temp-end).count()/1000000.0f;
       tick=1;
    }
}

void Config::MeasureTransfer() {
    int averageFramCount=10;
    static int tick=0;
    if(tick==0){
        transferStart=std::chrono::high_resolution_clock::now();
        tick=1;
    }
    else if(tick==1){
        transferEnd=std::chrono::high_resolution_clock::now();
        float currentTime=(float)(transferEnd-transferStart).count()/1000000.0f;
        transferTime*=averageFramCount-1;
        transferTime+=currentTime;
        transferTime/=averageFramCount;
        tick=0;
    }
}