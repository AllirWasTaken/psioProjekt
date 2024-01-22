#include "TaskStack.h"
#include <memory.h>



TaskStack::TaskStack(int size){
    data=new Point2[size];
    top=0;
    this->size=size;
}

TaskStack::~TaskStack(){
    delete[] data;
}

void TaskStack::IncreaseSize(){
    Point2 *temp=new Point2[size*2];
    memcpy(temp,data,sizeof(Point2)*size);
    size*=2;
    delete[] data;
    data=temp;
}


void TaskStack::Pop(Point2& a){
    top--;
    a=data[top];
}

void TaskStack::Push(Point2& a){
    if(top==size){
        IncreaseSize();
    }
    data[top]=a;
    top++;
}

bool TaskStack::isEmpty(){
    return !top;
}

void TaskStack::EmptyTheStack(){
    top=0;
}

Point2& TaskStack::operator[](int index){
    return data[index];
}

int TaskStack::GetSize(){
    return top;
}

Point2* TaskStack::Data(){
    return data;
}