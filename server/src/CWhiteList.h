#pragma once

#include "sqlite/sqlite3.h"

class CWhiteList : public CSingleton<CWhiteList>
{
private:
	std::unordered_map<std::string, bool> m_acceptList;
	sqlite3* m_pDatabase;


public:
	CWhiteList();
	~CWhiteList();

	bool Initialize();
	
	bool TryGetList();
	bool Add(std::string strIp);
	bool Remove(std::string strIp);

	bool IsClientCanConnect(SocketAddress* pClient);
	bool IsIpCanConnect(std::string& strIp);

	static int OnWhiteListLoad(void* pNotUsed, int argc, char** argv, char** aszColumnName);
	static int OnIpAdd(void* pNotUsed, int argc, char** argv, char** aszColumnName);
	static int OnIpRemove(void* pNotUsed, int argc, char** argv, char** aszColumnName);
};
