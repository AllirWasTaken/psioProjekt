#pragma once
#include <chrono>

class Config{
public:
//work control
bool work;
//Fps measurement
float fps;
float time;
std::chrono::high_resolution_clock::time_point start,end;
void MeasureFps();
void MeasureTime();
//Settings
void SetDefault();
int videoY;
int videoX;
int videoWorkY;
int videoWorkX;

int calibrationMode;
int debugMode;

int edgeRange;
int edgeDetectionThreshold;
int blobEdgesAmount;
int filterNoiseThreshold;
int antialiasingIterations;

int ObjectNoiseThreshold;
int detectObjects;





};