#include "Sockets.h"
#include <stdexcept>
#include <iostream>


SocketServer::SocketServer(int port){

   
    //Library initialization
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		throw std::runtime_error("Failed to initalise WSA library");
	}
		
	//Opening Socket
	if((s = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) == INVALID_SOCKET)
	{
        throw std::runtime_error("Failed to create a socket");
	}

    //Prerequesties to bind
    memset( & service, 0, sizeof( service ) );
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    service.sin_port = htons( port );


    //binding and listening with socket
    if( bind(s,( SOCKADDR * ) & service, sizeof( service ) ) == SOCKET_ERROR )
    {
        throw std::runtime_error("Failed to bind socket");
        closesocket(s);
    }
    else isSocektOpen=true;

    if( listen(s, 1 ) == SOCKET_ERROR ){
        throw std::runtime_error("Failed to start listening on socket");
    }

}


SocketServer::~SocketServer(){
    if(isSocektOpen){
        closesocket(s);
    }
}

void SocketServer::WaitForClientToConnect(){
    client = accept(s, NULL, NULL);
}

int  SocketServer::ReciveData(std::vector<uint8_t>& dataVector){
    int messageSize;
    recv(client,(char*)&messageSize,4,0);
    if(messageSize==CLOSE_MESSAGE_SIZE)return 1;
    dataVector.resize(messageSize);
    recv(client,(char*)dataVector.data(),messageSize,0);
    return 0;
}

void SocketServer::SendData(std::vector<uint8_t>& dataVector){
    int messageSize=dataVector.size();
    send(client,(char*)&messageSize,4,0);
    int test=send(client,(char*)dataVector.data(),messageSize,0);
    if(test==INVALID_SOCKET){
        throw std::runtime_error("Client unsafely dissconected");
    }

}