#include "Image.h"
#include <cmath>


Pixel& Pixel::operator=(uint8_t a){
    r=a;
    g=a;
    b=a;
    return *this;
}




Image::Image(int x,int y){
    imageData.resize(y);
    for(int i=0;i<y;i++){
        imageData[i].resize(x);
    }

    tempImageData.resize(y);
    for(int i=0;i<y;i++){
        tempImageData[i].resize(x);
    }

    imageX=x;
    imageY=y;
}

std::vector<Pixel>& Image::operator[](int index){
    return imageData[index];
}

void Image::ConvertStreamToImage(std::vector<uint8_t>& stream){
    int streamCounter=0;
    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            imageData[y][x].b=stream[streamCounter+0];
            imageData[y][x].g=stream[streamCounter+1];
            imageData[y][x].r=stream[streamCounter+2];
            streamCounter+=3;
        }
    }
}

void Image::ConvertImageToStream(std::vector<uint8_t>& stream){
    int streamCounter=0;
    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            stream[streamCounter+0]=imageData[y][x].b;
            stream[streamCounter+1]=imageData[y][x].g;
            stream[streamCounter+2]=imageData[y][x].r;
            streamCounter+=3;
        }
    }
}

void Image::CutImage(Image& dest){
    int ydif=imageY-dest.imageY;
    int xdif=imageX-dest.imageX;
    ydif/=2;
    xdif/=2;

    for(int y=0;y<dest.imageY;y++){
        for(int x=0;x<dest.imageX;x++){
            dest[y][x]=imageData[y+ydif][x+xdif];
        }
    }
}

void Image::FitIntoImage(Image& src){
    int ydif=imageY-src.imageY;
    int xdif=imageX-src.imageX;
    ydif/=2;
    xdif/=2;

    for(int y=0;y<src.imageY;y++){
        for(int x=0;x<src.imageX;x++){
            imageData[y+ydif][x+xdif]=src[y][x];
        }
    }
}

int Image::MaxMeanValueDiffAroundPixel(int x, int y){
    int result=0;
    //up
    if(y>0){
        result+=abs(imageData[y-1][x].r-imageData[y][x].r);
        result+=abs(imageData[y-1][x].g-imageData[y][x].g);
        result+=abs(imageData[y-1][x].b-imageData[y][x].b);
    }
    //down
    if(y<imageY-1){
        result+=abs(imageData[y+1][x].r-imageData[y][x].r);
        result+=abs(imageData[y+1][x].g-imageData[y][x].g);
        result+=abs(imageData[y+1][x].b-imageData[y][x].b);
    }
    //right
    if(x<imageX-1){
        result+=abs(imageData[y][x+1].r-imageData[y][x].r);
        result+=abs(imageData[y][x+1].g-imageData[y][x].g);
        result+=abs(imageData[y][x+1].b-imageData[y][x].b);
    }
    //left
    if(x>0){
        result+=abs(imageData[y][x-1].r-imageData[y][x].r);
        result+=abs(imageData[y][x-1].g-imageData[y][x].g);
        result+=abs(imageData[y][x-1].b-imageData[y][x].b);
    }
    if(result>255)result=255;
    return result;
}

void Image::EdgeDetection(int tolerance){
    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            tempImageData[y][x]=0;
        }
    }

    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            tempImageData[y][x]=MaxMeanValueDiffAroundPixel(x,y);
        }
    }

    if(tolerance!=-1){
        for(int y=0;y<imageY;y++){
            for(int x=0;x<imageX;x++){
                if(tempImageData[y][x].r>tolerance)tempImageData[y][x]=EDGE_COLOR;
                else tempImageData[y][x]=0;
            }
        }
    }

    imageData=tempImageData;
}

int Image::BlobingMaxValueAround(int x,int y, int currentVal){
    if(tempImageData[y][x].r>currentVal)return tempImageData[y][x].r;
    //up
    if(y>0){
        if(tempImageData[y-1][x].r>currentVal)return currentVal;
    }
    //down
    if(y<imageY-1){
if(tempImageData[y+1][x].r>currentVal)return currentVal;
    }
    //right
    if(x<imageX-1){
       if(tempImageData[y][x+1].r>currentVal)return currentVal;
    }
    //left
    if(x>0){
      if(tempImageData[y][x-1].r>currentVal)return currentVal;
    }
    return 0;
}

void Image::BlobEdges(int blobing){
    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            if(imageData[y][x].r==EDGE_COLOR)tempImageData[y][x].r=blobing;
        }
    }
    for(int i=blobing-1;i>=0;i--){
        for(int y=0;y<imageY;y++){
            for(int x=0;x<imageX;x++){
                tempImageData[y][x].r=BlobingMaxValueAround(x,y,i);
            }
        }
    }
    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            if(tempImageData[y][x].r>0)tempImageData[y][x]=EDGE_COLOR;
        }
    }
    imageData=tempImageData;
}

void Image::AddBorderingToStack(Point2& pix,TaskStack& stack){

    int x,y;
    y=pix.y;
    x=pix.x;

    Point2 temp;
    //up
    if(y>0){
        if(tempImageData[y-1][x].r==EDGE_COLOR){
            temp.x=x;
            temp.y=y-1;
            stack.Push(temp);
        }
    }
    //down
    if(y<imageY-1){
        if(tempImageData[y+1][x].r==EDGE_COLOR){
            temp.x=x;
            temp.y=y+1;
            stack.Push(temp);
        }
    }
    //right
    if(x<imageX-1){
       if(tempImageData[y][x+1].r==EDGE_COLOR){
            temp.x=x+1;
            temp.y=y;
            stack.Push(temp);
        }
    }
    //left
    if(x>0){
      if(tempImageData[y][x-1].r==EDGE_COLOR){
            temp.x=x-1;
            temp.y=y;
            stack.Push(temp);
        }
    }
}

void Image::FilterOutNoise(int minimumPixelThreshold){
    tempImageData=imageData;
    Point2 tempPoint;



    for(int y=0;y<imageY;y++){
        for(int x=0;x<imageX;x++){
            if(tempImageData[y][x].r==EDGE_COLOR){
                TaskStack objectStack;
                tempPoint.y=y;
                tempPoint.x=x;
                taskStack.Push(tempPoint);
                Point2 currentPoint;
                while(!taskStack.isEmpty()){
                    taskStack.Pop(currentPoint);
                    objectStack.Push(currentPoint);
                    tempImageData[currentPoint.y][currentPoint.x].r=0;
                    AddBorderingToStack(currentPoint,taskStack);
                }
                if(objectStack.GetSize()<minimumPixelThreshold){
                    while(!objectStack.isEmpty()){
                        objectStack.Pop(currentPoint);
                        imageData[currentPoint.y][currentPoint.x]=0;
                    }
                }
            }
        }
    }

}