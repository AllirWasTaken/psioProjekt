#include "Config.h"



void Config::SetDefault(){
    videoY=720;
    videoX=1280;
    videoWorkY=700;
    videoWorkX=1200;

    calibrationMode=0;
    debugMode=0;
    analyzeBackground=0;

    edgeDetectionThreshold1=180;
    edgeDetectionThreshold2=90;
    blobEdgesAmount=2;
    filterNoiseThreshold=500;
    backgroundTolerance=25;

    ObjectNoiseThreshold=5000;
    detectObjects=1;

    typeThreshold=70;
    percentageOfObjectColorAnalyzed=10;



    end=std::chrono::high_resolution_clock::now();
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