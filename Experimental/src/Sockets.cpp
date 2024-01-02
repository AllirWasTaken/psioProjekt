#include "Sockets.h"

#include <stdexcept>
#include <iostream>

#include "SocketErrors.h"

namespace
{

void sendAll(SOCKET socket, char *buffer, int size, int flags = 0)
{
    int offset = 0;
    do
    {
        int n = send(socket, buffer + offset, size - offset, flags);
        if (n == INVALID_SOCKET)
            throw InvalidSocketError{};
        offset += n;
    } while (offset < size);
}

void recvAll(SOCKET socket, char *buffer, int size, int flags = 0)
{
    int offset = 0;
    do
    {
        int n = recv(socket, buffer + offset, size - offset, flags);
        if (n == INVALID_SOCKET)
            throw InvalidSocketError{};
        offset += n;
    } while (offset < size);
}

} // namespace

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
        closesocket(s);
        throw std::runtime_error("Failed to bind socket");
    }
    else isSocketOpen=true;

    if( listen(s, 1 ) == SOCKET_ERROR ){
        throw std::runtime_error("Failed to start listening on socket");
    }

}

SocketServer::~SocketServer()
{
    closeSocket();
}


void SocketServer::closeSocket()
{
    if (!isSocketOpen)
        return;
    isSocketOpen = false;
    closesocket(s);
}

void SocketServer::shutdown()
{
    closeSocket();
    ::shutdown(client, SD_BOTH);
}

void SocketServer::WaitForClientToConnect(){
    client = accept(s, NULL, NULL);
}

int  SocketServer::ReciveData(std::vector<uint8_t>& dataVector){
    int messageSize;
    recvAll(client, (char *)&messageSize, 4);
    if (messageSize == CLOSE_MESSAGE_SIZE)
        return 1;
    dataVector.resize(messageSize);
    recvAll(client, (char *)dataVector.data(), messageSize);
    return 0;
}

void SocketServer::SendData(std::vector<uint8_t>& dataVector){
    int messageSize = dataVector.size();
    sendAll(client, (char *)&messageSize, 4);
    sendAll(client, (char *) dataVector.data(), messageSize);
}