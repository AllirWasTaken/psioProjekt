#include <iostream>
#include "Sockets.h"
#include "windows.h"




int main(void){

    SocketServer server(12345);
    std::cout<<"BABA!"<<std::endl;


    Sleep(1000*5);


    return 0;
}