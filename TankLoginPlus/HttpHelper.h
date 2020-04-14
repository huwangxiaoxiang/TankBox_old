#pragma once
#include <winsock.h>
#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
#include <string>
#include<vector>

class HttpHelper
{

public :
	HttpHelper();
	std::string postData(std::string host, std::string path, std::string post_content);

	std::string getData(std::string host, std::string path, std::string get_content);

	static std::vector<std::string> split_str(std::string target, std::string pattern);

	static std::wstring UTF8ToUnicode(const std::string& str);
	
	static std::string wstringTostring(const std::wstring& str);


protected:
	std::string socketHttp(std::string host, std::string request);

};

