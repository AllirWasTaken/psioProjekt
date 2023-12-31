#include "TaskStack.h"



TaskStack::TaskStack(){
    data.resize(100);
    top=0;
}



void TaskStack::Pop(Point2& a){
    top--;
    a=data[top];
}

void TaskStack::Push(Point2& a){
    if(top==data.size()){
        data.resize(data.size()*2);
    }
    data[top]=a;
    top++;
}

bool TaskStack::isEmpty(){
    if(!top)return true;
    return false;
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
