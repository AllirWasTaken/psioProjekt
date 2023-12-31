#pragma once
#include <chrono>

class Config{
public:
//work control
bool work;
//Fps measurement
float fps;
std::chrono::high_resolution_clock::time_point start,end;
void MeasureFps();
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





};