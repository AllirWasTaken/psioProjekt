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

    currentObject="kiwi.bmp";
    currentPath=path+currentObject;
    kiwi.LoadBmpImage(currentPath.data());

    currentObject="avocado.bmp";
    currentPath=path+currentObject;
    avocado.LoadBmpImage(currentPath.data());

}

void Labeler::Label(Image &image, Object &objectVar) {
    image.DrawSquare(objectVar.pos.x,objectVar.pos.y,objectVar.width,objectVar.height);
    if(objectVar.type==OBJECT){
        image.FitIntoImage(object,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-object.Height());
    }
    else if(objectVar.type==APPLE){
        image.FitIntoImage(apple,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-apple.Height());
    }
    else if(objectVar.type==ORANGE){
        image.FitIntoImage(orange,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-orange.Height());
    }
    else if(objectVar.type==KIWI){
        image.FitIntoImage(kiwi,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-kiwi.Height());
    }else if(objectVar.type==CITRUS){
        image.FitIntoImage(citrus,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-citrus.Height());
    }
    else if(objectVar.type==AVOCADO){
        image.FitIntoImage(avocado,objectVar.pos.x-objectVar.width/2,objectVar.pos.y-objectVar.height/2-avocado.Height());
    }
}