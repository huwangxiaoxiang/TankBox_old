#include "pch.h"
#include "HttpHelper.h"
#include<iostream>
#include<sstream>
#include <Ws2tcpip.h>
#include<string.h>
#include <locale>
#include <codecvt>

HttpHelper::HttpHelper()
{
#ifdef WIN32
	//此处一定要初始化一下，否则gethostbyname返回一直为空

	WSADATA wsa = { 0 };
	int resu=WSAStartup(MAKEWORD(2, 2), &wsa);
	std::cout << resu << std::endl;
#endif
}

std::string HttpHelper::socketHttp(std::string host, std::string request)
{
	int sockfd;
	struct sockaddr_in address;
	struct hostent* server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		OutputDebugString(L"创建Socket失败\n");
		return "";
	}
	address.sin_family = AF_INET;
	address.sin_port = htons(80);
	
	server = gethostbyname(host.c_str());
	if (server == NULL) {
		OutputDebugString(L"域名解析失败!\n");
		return "";
	}
	else {
		memcpy((char*)& address.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
		
	}
	
	if (-1 == connect(sockfd, (struct sockaddr*) & address, sizeof(address))) {
		OutputDebugString(L"连接服务器失败\n");
		return "";
	}

	//DBG << request << std::endl;
#ifdef WIN32
	send(sockfd, request.c_str(), request.size(), 0);
#else
	write(sockfd, request.c_str(), request.size());
#endif
	std::stringstream result;
	int rc;
	char* buf = new char[1024];

#ifdef WIN32
	while (rc = recv(sockfd, buf, 1024, 0))
#else
	while (rc = read(sockfd, buf + offset, 1024))
#endif
	{
		if (rc < 1024) buf[rc] = '\0';
		result << buf;
		buf = new char[1024];
	}

#ifdef WIN32
	closesocket(sockfd);
#else
	close(sockfd);
#endif
	return result.str();
}

std::string HttpHelper::postData(std::string host, std::string path, std::string post_content)
{
	//POST请求方式
	std::stringstream stream;
	stream << "POST " << path;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Content-Type:application/x-www-form-urlencoded\r\n";
	stream << "Content-Length:" << post_content.length() << "\r\n";
	stream << "Connection:close\r\n\r\n";
	stream << post_content.c_str();

	return socketHttp(host, stream.str());
}

std::string HttpHelper::getData(std::string host, std::string path, std::string get_content)
{
	//GET请求方式
	std::stringstream stream;
	stream << "GET " << path << "?" << get_content;
	stream << " HTTP/1.0\r\n";
	stream << "Host: " << host << "\r\n";
	stream << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
	stream << "Connection:close\r\n\r\n";
	
	
	return socketHttp(host, stream.str());
}

 std::vector<std::string> HttpHelper::split_str(std::string target, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	target += pattern;//扩展字符串以方便操作
	int size = target.size();

	for (int i = 0; i < size; i++)
	{
		pos = target.find(pattern, i);
		if (pos < size)
		{
			std::string s = target.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

 std::wstring HttpHelper::UTF8ToUnicode(const std::string& str)
 {
	 std::wstring ret;
	 try {
		 std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		 ret = wcv.from_bytes(str);
	 }
	 catch (const std::exception& e) {
		 std::cerr << e.what() << std::endl;
	 }
	 return ret;
 }

 std::string HttpHelper::wstringTostring(const std::wstring& wstr)
 {
	 std::string result;
	 //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	 int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	 char* buffer = new char[len + 1];
	 //宽字节编码转换成多字节编码  
	 WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	 buffer[len] = '\0';
	 //删除缓冲区并返回值  
	 result.append(buffer);
	 delete[] buffer;
	 return result;
 }
