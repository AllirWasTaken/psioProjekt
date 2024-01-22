#include "Image.h"
#include <cmath>
#include <cstdio>
#include <opencv2/opencv.hpp>
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

void Image::ConvertStreamToImage(const unsigned char* stream,int colorChanelCount){
    int streamCounter=0;
    if(colorChanelCount==3) {
        for (int y = 0; y < imageY; y++) {
            for (int x = 0; x < imageX; x++) {
                imageData[y][x].b = stream[streamCounter + 0];
                imageData[y][x].g = stream[streamCounter + 1];
                imageData[y][x].r = stream[streamCounter + 2];
                streamCounter += 3;
            }
        }
    }
    else if(colorChanelCount==1){
        for (int y = 0; y < imageY; y++) {
            for (int x = 0; x < imageX; x++) {
                imageData[y][x] = stream[streamCounter];
                streamCounter += 1;
            }
        }
    }
}

void Image::ConvertImageToStream(unsigned char* stream){
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

void Image::CutImage(Image& dest,int beginX,int beginY){
    int ydif,xdif;

    if(beginY==-1) {
        ydif = imageY - dest.imageY;
        ydif/=2;
    }
    else {
        xdif=beginX;
    }
    if(beginX==-1) {
        xdif = imageX - dest.imageX;
        xdif/=2;
    }
    else{
        ydif=beginY;
    }



    for(int y=0;y<dest.imageY;y++){
        for(int x=0;x<dest.imageX;x++){
            dest[y][x]=imageData[y+ydif][x+xdif];
        }
    }
}

void Image::FitIntoImage(Image& src,int beginX,int beginY){
    int ydif,xdif;

    if(beginY==-1) {
        ydif = imageY - src.imageY;
        ydif/=2;
    }
    else {
        xdif=beginX;
    }
    if(beginX==-1) {
        xdif = imageX - src.imageX;
        xdif/=2;
    }
    else{
        ydif=beginY;
    }

    int posX,posY;
    for(int y=0;y<src.imageY;y++){
        for(int x=0;x<src.imageX;x++){
            posX=x+xdif;
            posY=y+ydif;
            if(posY<imageY-1&&posY>0&&posX>0&&posX<imageX-1) {
                imageData[posY][posX] = src[y][x];
            }
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
    //up left
    if(y>0&&x>0){
        result+=abs(imageData[y-1][x-1].r-imageData[y][x].r);
        result+=abs(imageData[y-1][x-1].g-imageData[y][x].g);
        result+=abs(imageData[y-1][x-1].b-imageData[y][x].b);
    }
    //up right
    if(x<imageX-1&&y>0){
        result+=abs(imageData[y-1][x+1].r-imageData[y][x].r);
        result+=abs(imageData[y-1][x+1].g-imageData[y][x].g);
        result+=abs(imageData[y-1][x+1].b-imageData[y][x].b);
    }
    //down left
    if(y<imageY-1&&x>0){
        result+=abs(imageData[y+1][x-1].r-imageData[y][x].r);
        result+=abs(imageData[y+1][x-1].g-imageData[y][x].g);
        result+=abs(imageData[y+1][x-1].b-imageData[y][x].b);
    }
    //down right
    if(y<imageY-1&&x<imageX-1){
        result+=abs(imageData[y+1][x+1].r-imageData[y][x].r);
        result+=abs(imageData[y+1][x+1].g-imageData[y][x].g);
        result+=abs(imageData[y+1][x+1].b-imageData[y][x].b);
    }

    result/=2;

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

    if(tolerance){
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
    tempImageData=imageData;
    if(!blobing)return;
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
    //up left
    if(y>0&&x>0){
        if(tempImageData[y-1][x-1].r==EDGE_COLOR){
            temp.x=x-1;
            temp.y=y-1;
            stack.Push(temp);
        }
    }
    //up right
    if(x<imageX-1&&y>0){
        if(tempImageData[y-1][x+1].r==EDGE_COLOR){
            temp.x=x+1;
            temp.y=y-1;
            stack.Push(temp);
        }
    }
    //down left
    if(y<imageY-1&&x>0){
        if(tempImageData[y+1][x-1].r==EDGE_COLOR){
            temp.x=x-1;
            temp.y=y+1;
            stack.Push(temp);
        }
    }
    //down right
    if(y<imageY-1&&x<imageX-1){
        if(tempImageData[y+1][x+1].r==EDGE_COLOR){
            temp.x=x+1;
            temp.y=y+1;
            stack.Push(temp);
        }
    }
}

void Image::FilterOutNoise(TaskStack &taskStack,int minimumPixelThreshold){
    if(!minimumPixelThreshold)return;
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

int Image::GetWhitePixelsAround(int x,int y){
    int result=0;
    Point2 temp;
    //up
    if(y>0){
        if(imageData[y-1][x].r==EDGE_COLOR){
            result++;
        }
    }
    //up left
    if(y>0&&x>0){
        if(imageData[y-1][x-1].r==EDGE_COLOR){
            result++;
        }
    }
    //up right
    if(x<imageX-1&&y>0){
        if(imageData[y-1][x+1].r==EDGE_COLOR){
            result++;
        }
    }
    //down left
    if(y<imageY-1&&x>0){
        if(imageData[y+1][x-1].r==EDGE_COLOR){
            result++;
        }
    }
    //down right
    if(y<imageY-1&&x<imageX-1){
        if(imageData[y+1][x+1].r==EDGE_COLOR){
            result++;
        }
    }
    //down
    if(y<imageY-1){
        if(imageData[y+1][x].r==EDGE_COLOR){
            result++;
        }
    }
    //right
    if(x<imageX-1){
       if(imageData[y][x+1].r==EDGE_COLOR){
            result++;
        }
    }
    //left
    if(x>0){
      if(imageData[y][x-1].r==EDGE_COLOR){
            result++;
        }
    }
    return result;
}

void Image::Antialiasing(int iterations){

    for(int i=0;i<iterations;i++){
         for(int y=0;y<imageY;y++){
            for(int x=0;x<imageX;x++){
                tempImageData[y][x].r=GetWhitePixelsAround(x,y);
            }
        }

        for(int y=0;y<imageY;y++){
            for(int x=0;x<imageX;x++){
                if(tempImageData[y][x].r<5){
                    imageData[y][x]=0;
                }
            }
        }
    }

}

int Image::Height(){
    return imageY;
}

int Image::Width(){
    return imageX;
}


void Image::DrawSquare(int x,int y,int width,int height){
    x-=width/2;
    y-=height/2;
    int posX,posY;
    for(int j=0;j<height;j++){
        posY=j+y;
        posX=x;
        if(posY<imageY-1&&posY>0&&posX>0&&posX<imageX-1) {
            imageData[posY][posX] = 0;
            imageData[posY][posX].g = 255;
        }
        posX=x+width;
        if(posY<imageY-1&&posY>0&&posX>0&&posX<imageX-1) {
            imageData[posY][posX] = 0;
            imageData[posY][posX].g = 255;
        }
    }
    for(int i=0;i<width;i++){
        posY=y;
        posX=i+x;
        if(posY<imageY-1&&posY>0&&posX>0&&posX<imageX-1) {
            imageData[y][i + x] = 0;
            imageData[y][i + x].g = 255;
        }
        posY=y+height;
        if(posY<imageY-1&&posY>0&&posX>0&&posX<imageX-1) {
            imageData[y + height][i + x] = 0;
            imageData[y + height][i + x].g = 255;
        }
    }
}

void Image::ConvertCannyDetectionToNormal() {
    for(int y=0;y<imageY;y++) {
        for (int x = 0; x < imageX; x++) {
            if(imageData[y][x].r)imageData[y][x]=EDGE_COLOR;
        }
    }
}

struct __attribute__((packed, aligned(1))) BmpHeader{
    uint16_t signature;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t data_offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size_bytes;
    uint32_t x_resolution;
    uint32_t y_resolution;
    uint32_t number_of_colors;
    uint32_t important_colors;
};

struct  __attribute__((packed, aligned(1))) bitPixel{
    uint8_t b,g,r;
};

void Image::LoadBmpImage(const char *path) {
    FILE* f =fopen(path,"rb");
    if(!f)return;
    BmpHeader header;
    fread(&header,sizeof(BmpHeader),1,f);

    Image temp(header.width,header.height);
    bitPixel *data=new bitPixel[header.width*header.height];
    fread(data,sizeof(bitPixel),header.width*header.height,f);
    fclose(f);

    int dataCounter=0;
    for(int y=0;y<header.height;y++){
        for(int x=0;x<header.width;x++){
            temp[header.height-y-1][x].r=data[dataCounter].r;
            temp[header.height-y-1][x].g=data[dataCounter].g;
            temp[header.height-y-1][x].b=data[dataCounter].b;
            dataCounter++;
        }
    }
    delete[] data;



    *this=temp;

}

Image &Image::operator=(Image &image) {
    imageX=image.imageX;
    imageY=image.imageY;
    imageData=image.imageData;
    tempImageData=image.tempImageData;
    return *this;
}