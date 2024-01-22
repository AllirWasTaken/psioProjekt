#include "Labeler.h"
#include <string>
Labeler::Labeler(const char *path) {
    std::string currentPath;
    std::string currentObject="object.bmp";
    currentPath=path+currentObject;
    object.LoadBmpImage(currentPath.data());
    currentObject="apple.bmp";
    currentPath=path+currentObject;
    apple.LoadBmpImage(currentPath.data());
    currentObject="orange.bmp";
    currentPath=path+currentObject;
    orange.LoadBmpImage(currentPath.data());
    currentObject="citrus.bmp";
    currentPath=path+currentObject;
    citrus.LoadBmpImage(currentPath.data());

}

void Labeler::Label(Image &image, Object &objectVar) {
    image.DrawSquare(objectVar.pos.x,objectVar.pos.y,objectVar.width,objectVar.height);
    if(objectVar.type==0){
        image.FitIntoImage(object,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-object.Height());
    }
    else if(objectVar.type==1){
        image.FitIntoImage(apple,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-apple.Height());
    }
    else if(objectVar.type==2){
        image.FitIntoImage(orange,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-orange.Height());
    }
    else if(objectVar.type==3){
        image.FitIntoImage(banana,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-banana.Height());
    }else if(objectVar.type==4){
        image.FitIntoImage(citrus,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-citrus.Height());
    }
}