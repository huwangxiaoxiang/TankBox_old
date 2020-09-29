#include "pch.h"
#include "ServerListManager.h"

ServerList ServerListManager::servers = ServerList();

void ServerListManager::Inistance()
{
	servers.AddServer(CString("华南一区"), 1);
	servers.AddServer(CString("华南二区"), 4);
	servers.AddServer(CString("华南三区"), 7);
	servers.AddServer(CString("华东一区"), 2);
	servers.AddServer(CString("华东二区"), 3);
	servers.AddServer(CString("华东三区"), 6);
	servers.AddServer(CString("华北一区"), 5);
	servers.AddServer(CString("联盟一区"), 5000);
	servers.AddServer(CString("冒险岛特区"), 8);
}

int ServerListManager::GetServerId(CString name)
{
	int m = servers.GetServerId(name);
	if (m < 0) return -1;
	else return m;
}

std::vector<CString> ServerListManager::GetServerNameList()
{
	return servers.GetNameList();
}
