#include "pch.h"
#include "ServerList.h"

void ServerList::AddServer(CString name, int serverId)
{
	this->lists[name] = serverId;
}

int ServerList::GetServerId(CString name)
{
	return this->lists[name];
}

void ServerList::RemoveServer(CString name)
{
	this->lists.erase(name);
}

void ServerList::Clear()
{
	this->lists.clear();
}

std::vector<CString> ServerList::GetNameList()
{
	std::vector<CString> result;
	
	for (auto& x : this->lists) {
		result.push_back(x.first);
	}
	
	return result;
}
