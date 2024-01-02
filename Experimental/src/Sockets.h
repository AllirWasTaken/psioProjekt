#pragma once
#include <atomic>
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
    std::atomic<bool> isSocketOpen = false;

public:
    SocketServer(int port);
    ~SocketServer();
    void closeSocket();
    void shutdown();
    void WaitForClientToConnect();
    void SendData(std::vector<uint8_t>& dataVector);
    int ReciveData(std::vector<uint8_t>& dataVector);
};