
#include <valarray>
#include "Predictor.h"

Predictor* instance;

Predictor::Predictor() {
    instance=this;
}

int Predictor::Get(FruitType a) {
    return instance->count[a];
}

void Predictor::Set() {
    count[0]=0;
    count[1]=0;
    count[2]=0;
    count[3]=0;
    count[4]=0;
    count[5]=0;
}


void Predictor::UpdatePredictor(std::vector<Object> &obj) {
    for(int i=0;i<vec.size();i++){
        vec[i].alive=false;
    }
    for(int i=0;i<obj.size();i++){
        CheckObject(obj[i]);
    }

    for(int i=0;i<vec.size();i++){
        if(!vec[i].alive){
            vec[i].life--;
            if(vec[i].life<0) {
                vec.erase(vec.begin() + i);
                i--;
            }
        }
        else {
            vec[i].life++;
            if(vec[i].life>10)vec[i].life=10;
        }
    }
}


void Predictor::CheckObject(Object &obj) {
    float distance;
    bool match= false;
    for(int i=0;i<vec.size();i++){
        distance=sqrt(pow((float)(obj.pos.x-vec[i].middle.x),2)+pow((float)(obj.pos.y-vec[i].middle.y),2));
        if(distance<MAX_SPEED){
            match=true;
            vec[i].alive=true;
            vec[i].middle=obj.pos;
            if(obj.type!=OBJECT) {
                if (obj.type == vec[i].type) {
                    vec[i].resistance++;
                    if (vec[i].resistance > 300)vec[i].resistance = 300;
                } else {
                    vec[i].resistance--;
                    if (vec[i].resistance < 0) {
                        vec[i].resistance = 0;
                        Del(vec[i].type);
                        vec[i].type = obj.type;
                        Add(obj.type);
                    } else {
                        obj.type = vec[i].type;
                    }
                }
            }
        }
    }
    if(!match){
        Predictions newP{
            .middle=obj.pos,
            .type=obj.type,
            .resistance=0,
            .alive=true
        };
        Add(newP.type);
        vec.push_back(newP);

    }
}

void Predictor::Add(FruitType type) {
    count[type]++;
}
void Predictor::Del(FruitType type) {
    count[type]--;
}

