#pragma once




class Point2{
    public:
    int x,y;
};


class TaskStack{
    private:
    Point2 *data;
    int top,size;
    void IncreaseSize();
    

    public:
    TaskStack();
    ~TaskStack();
    void Pop(Point2& a);
    void Push(Point2& a);
    void EmptyTheStack();
    Point2& operator[](int index);
    bool isEmpty();
    int GetSize();


};