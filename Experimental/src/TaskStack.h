#pragma once
#include<vector>



class Point2{
    public:
    int x,y;
};


class TaskStack{
    private:
    std::vector<Point2> data;
    int top;
    

    public:
    TaskStack();
    void Pop(Point2& a);
    void Push(Point2& a);
    bool isEmpty();


};