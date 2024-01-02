#include "ObjectDetection.h"
#include "memory.h"


void ObjectDetection::DeepSearchSetValue(Image& image,TaskStack& stack,int find,int set){
    Point2 temp;
    while(!stack.isEmpty()){
        stack.Pop(temp);
        int y=temp.y;
        int x=temp.x;
        int imageY=image.Height();
        int imageX=image.Width();
        image[y][x].r=set;
        if(y>0){
            if(image[y-1][x].r==find){
                temp.x=x;
                temp.y=y-1;
                stack.Push(temp);
            }
        }
        //down
        if(y<imageY-1){
            if(image[y+1][x].r==find){
                temp.x=x;
                temp.y=y+1;
                stack.Push(temp);
            }
        }
        //right
        if(x<imageX-1){
        if(image[y][x+1].r==find){
                temp.x=x+1;
                temp.y=y;
                stack.Push(temp);
            }
        }
        //left
        if(x>0){
        if(image[y][x-1].r==find){
                temp.x=x-1;
                temp.y=y;
                stack.Push(temp);
            }
        }
    }
}


void ObjectDetection::DeepSearchSetValueAndAddToStack(Image& image, TaskStack& stack, TaskStack& resultStack, int find,int set){
    Point2 temp;
    while(!stack.isEmpty()){
        stack.Pop(temp);
        int y=temp.y;
        int x=temp.x;
        int imageY=image.Height();
        int imageX=image.Width();
        image[y][x].r=set;
        resultStack.Push(temp);
        if(y>0){
            if(image[y-1][x].r==find){
                temp.x=x;
                temp.y=y-1;
                stack.Push(temp);
            }
        }
        //down
        if(y<imageY-1){
            if(image[y+1][x].r==find){
                temp.x=x;
                temp.y=y+1;
                stack.Push(temp);
            }
        }
        //right
        if(x<imageX-1){
        if(image[y][x+1].r==find){
                temp.x=x+1;
                temp.y=y;
                stack.Push(temp);
            }
        }
        //left
        if(x>0){
        if(image[y][x-1].r==find){
                temp.x=x-1;
                temp.y=y;
                stack.Push(temp);
            }
        }
    }
}


void ObjectDetection::DetectObjects(Image& image,int filterThreshold){
    objects.clear();
    for(int y=0;y<image.Height();y++){
        image[y][0].r=EDGE_COLOR;
        image[y][image.Width()-1].r=EDGE_COLOR;
    }

    for(int x=0;x<image.Width();x++){
        image[0][x].r=EDGE_COLOR;
        image[image.Height()-1][x].r=EDGE_COLOR;
    }

    TaskStack stack;
    Point2 currentPonit;


    //0 0 seeker (top left)
    image[0][0].r=0;
    image[1][0].r=0;
    image[0][1].r=0;
    currentPonit.x=0;
    currentPonit.y=0;
    stack.Push(currentPonit);

    //0 max seeker
    image[image.Height()-1][0].r=0;
    image[image.Height()-1][1].r=0;
    image[image.Height()-2][0].r=0;
    currentPonit.x=0;
    currentPonit.y=image.Height()-1;
    stack.Push(currentPonit);

    //max 0 seeker
    image[0][image.Width()-1].r=0;
    image[1][image.Width()-1].r=0;
    image[0][image.Height()-2].r=0;
    currentPonit.x=image.Width()-1;
    currentPonit.y=0;
    stack.Push(currentPonit);


    //max max seeker
    image[image.Height()-1][image.Width()-1].r=0;
    image[image.Height()-2][image.Width()-1].r=0;
    image[image.Height()-1][image.Height()-2].r=0;
    currentPonit.x=image.Width()-1;
    currentPonit.y=image.Height()-1;
    stack.Push(currentPonit);

    DeepSearchSetValue(image,stack,0,EDGE_COLOR);
    //Background is filled, time to make objects distinct

    for(int y=0;y<image.Height();y++){
        for(int x=0;x<image.Width();x++){

            if(image[y][x].r==0){
                currentPonit.x=x;
                currentPonit.y=y;
                TaskStack tempObjectStack;
                stack.Push(currentPonit);
                DeepSearchSetValueAndAddToStack(image,stack,tempObjectStack,0,EDGE_COLOR);
                if(tempObjectStack.GetSize()>=filterThreshold){
                    Object newObject;
                    newObject.bodyPixels.resize(tempObjectStack.GetSize());
                    memcpy(newObject.bodyPixels.data(),tempObjectStack.Data(),tempObjectStack.GetSize()*sizeof(Point2));
                    objects.push_back(newObject);
                }
            }

        }
    }

}

void ObjectDetection::DrawObjects(Image& image){
  for(int y=0;y<image.Height();y++){
        for(int x=0;x<image.Width();x++){
            image[y][x]=0;
        }
    }

    for(int i=0;i<objects.size();i++){
        for(int j=0;j<objects[i].bodyPixels.size();j++){
            image[objects[i].bodyPixels[j].y][objects[i].bodyPixels[j].x].b=EDGE_COLOR;
        }
    }
}

void ObjectDetection::CalculateObjectsVariables(){
    for(int i=0;i<objects.size();i++){
        objects[i].CalculateVariables();
    }
}

void Object::CalculateVariables(){
    maxPos=bodyPixels[0];
    minPos=bodyPixels[0];
    long long countX,countY;
    countX=0;
    countY=0;
    Point2 currentPoint;
    for(int i=0;i<bodyPixels.size();i++){
        currentPoint=bodyPixels[i];
        if(maxPos.y<currentPoint.y){
            maxPos.y=currentPoint.y;
        }
        if(maxPos.x<currentPoint.x){
            maxPos.x=currentPoint.x;
        }

        if(minPos.y>currentPoint.y){
            minPos.y=currentPoint.y;
        }
        if(minPos.x>currentPoint.x){
            minPos.x=currentPoint.x;
        }

        countX+=currentPoint.x;
        countY+=currentPoint.y;
    }

    pos.x=countX/bodyPixels.size();
    pos.y=countY/bodyPixels.size();

    height=maxPos.y-minPos.y;
    width=maxPos.x-minPos.y;

}

std::vector<Object>& ObjectDetection::GetObjects(){
    return objects;
}


void ObjectDetection::OffestObjects(int x,int y){
    for(int i=0;i<objects.size();i++){
        objects[i].maxPos.x+=x;
        objects[i].maxPos.y+=y;
        objects[i].minPos.x+=x;
        objects[i].minPos.y+=y;
        objects[i].pos.x+=x;
        objects[i].pos.y+=y;
    }
}