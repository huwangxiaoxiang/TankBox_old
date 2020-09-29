#pragma once
#include "ServerList.h"
class ServerListManager
{
private:
	static ServerList servers;
public:
	static void Inistance();
	static int GetServerId(CString name);
	static std::vector<CString> GetServerNameList();
};

