#pragma once
#include <chrono>

class Config{
public:
//work control
bool work;
//Fps measurement
float fps;
float time;
float transferTime;
std::chrono::high_resolution_clock::time_point start,end,transferStart,transferEnd;
void MeasureFps();
void MeasureTransfer();
//Settings
void SetDefault();
int videoY;
int videoX;
int videoWorkY;
int videoWorkX;

int calibrationMode;
int debugMode;

int edgeDetectionThreshold1;
int edgeDetectionThreshold2;
int blobEdgesAmount;
int filterNoiseThreshold;

int ObjectNoiseThreshold;
int detectObjects;





};