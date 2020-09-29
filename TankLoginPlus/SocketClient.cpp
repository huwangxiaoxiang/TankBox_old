
#include "pch.h"
#include "SocketClient.h"

void ReceiveMessage(SocketClient* client) {
	while (!client->is_exit) {
		try {
			std::stringstream ss;
			int length_sum = 0;
			do {
				char* buffer = new char[1024*1024];
				int length = recv(client->sockfd, buffer, 1024*1024, 0);
				if (length <= 0) break;
				length_sum += length;
				ss << buffer;
				client->OnReceiveByte(buffer, length);
				if (length < 1024 * 1024) {
					client->OnCompleteReceive();
					break;
				}
			} while (true);
			if (length_sum <= 0) break;
			client->OnReceiveMessage(ss.str());
			ss.str().c_str();
		}
		catch (int s) {
			client->is_exit = true;
			break;

		}
		closesocket(client->sockfd);

		OutputDebugString(L"客户端与服务端的连接已断开\n");
	}
}


SocketClient::SocketClient(int port, std::string ip)
{
	this->port = port;
	this->ip = ip;
}


void SocketClient::ConnectServer()
{
	struct sockaddr_in my_addr;

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(this->port);
	struct in_addr s;
	inet_pton(AF_INET, this->ip.c_str(), &s);
	my_addr.sin_addr = s;
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	WSAStartup(w_req, &wsadata);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sockfd, (SOCKADDR*)& my_addr, sizeof(my_addr)) == SOCKET_ERROR) {
		OutputDebugString(L"连接服务器失败\n");
		WSACleanup();
		return;
	}
	this->OnConnectEstablish();
	this->is_exit = false;
	std::thread receive_thread(ReceiveMessage,this);
	receive_thread.detach();
}

void SocketClient::DisConnect()
{
	this->is_exit = true;
	closesocket(this->sockfd);
	WSACleanup();
}

void SocketClient::SendData(std::string s)
{
	SendByte(s.c_str(), s.length());
}

void SocketClient::SendByte(const char* buffer, int size)
{
	int length=send(this->sockfd, buffer, size,0);
	if (length < size) {
		OutputDebugStringA("发送消息失败:");
		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");
	}
}

void SocketClient::OnConnectEstablish()
{
}

void SocketClient::OnReceiveMessage(std::string message)
{
}

void SocketClient::OnReceiveByte(char* buffer, int length)
{
}

void SocketClient::OnCompleteReceive()
{
}

void SocketClient::OnConnectBroken()
{
}
