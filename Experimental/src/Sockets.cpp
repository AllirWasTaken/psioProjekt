#include "Sockets.h"
#include <stdexcept>

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

void SocketServer::ReciveData(std::vector<uint8_t>& dataVector){
    int messageSize;
    recv(s,(char*)&messageSize,4,0);
    dataVector.resize(messageSize);
    recv(s,(char*)dataVector.data(),messageSize,0);
}