#pragma once
#include <vector>
#include <map>
class ServerList
{
private:
	std::map<CString, int> lists;
public:
	void AddServer(CString name, int serverId);
	int GetServerId(CString name);
	void RemoveServer(CString name);
	void Clear();
	std::vector<CString> GetNameList();
};

