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

int edgeDetectionThreshold;
int blobEdgesAmount;
int filterNoiseThreshold;
int antialiasingIterations;

int ObjectNoiseThreshold;
int detectObjects;





};