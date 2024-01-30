#pragma once
#include "ObjectDetection.h"


#define MAX_SPEED 150


struct Predictions{
    Point2 middle;
    FruitType type;
    int resistance;
    bool alive;
};

class Predictor {
private:
    int idCounter=0;
    std::vector<Predictions> vec;
    void CheckObject(Object &obj);
public:
    Predictor();
    void UpdatePredictor(std::vector<Object> &obj);
    void Add(FruitType type);
    void Del(FruitType type);
    void Set();
    int count[6];
    static int Get(FruitType a);

};


