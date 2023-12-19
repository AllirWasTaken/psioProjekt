#pragma once
#include <string>
#include <winsock2.h>
#include <cstdint>
#include <vector>

#define CLOSE_MESSAGE_SIZE 7312


class SocketServer{
    private:
    int port;
    WSADATA wsa;
	SOCKET s,client;
    sockaddr_in service;
    bool isSocektOpen=false;

    public:
    SocketServer(int port);
    ~SocketServer();
    void WaitForClientToConnect();
    void SendData(std::vector<uint8_t>& dataVector);
    int ReciveData(std::vector<uint8_t>& dataVector);


};