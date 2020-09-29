#pragma once
#include <string>
#include <thread>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

class SocketClient
{
private:
	std::string ip = "";
	int port = 0;
	
	
public:
	SocketClient(int port, std::string ip);
	bool is_exit = false;
	SOCKET sockfd;
	void ConnectServer();
	void DisConnect();
	void SendData(std::string s);
	void SendByte(const char* buffer, int size);

public:
	virtual void OnConnectEstablish();
	virtual void OnReceiveMessage(std::string message);
	virtual void OnReceiveByte(char* buffer, int length);
	virtual void OnCompleteReceive();
	virtual void OnConnectBroken();
};

