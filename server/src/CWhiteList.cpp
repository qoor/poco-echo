#include "StdInc.h"

CWhiteList* CSingleton<CWhiteList>::m_pInstance = nullptr;

CWhiteList::CWhiteList() : m_pDatabase(nullptr)
{

}

CWhiteList::~CWhiteList()
{
	if (m_pDatabase)
	{
		sqlite3_close(m_pDatabase);
		m_pDatabase = nullptr;
	}
}

bool CWhiteList::Initialize()
{
	sqlite3* pDatabase;
	int iResult = sqlite3_open("whitelist.sql", &pDatabase);

	if (iResult != SQLITE_OK)
	{
		std::cout << "화이트리스트 데이터베이스 연결 초기화에 실패했습니다. (code: " << iResult << "\n";
		return false;
	}

	m_pDatabase = pDatabase;
	return true;
}

bool CWhiteList::TryGetList()
{
	assert(m_pDatabase);

	int iResult;
	char* szErrMsg;

	// 테이블 생성
	{
		const char* szSql = "CREATE TABLE IF NOT EXISTS whitelist ("
							"id INTEGER PRIMARY KEY AUTOINCREMENT,"
							"Address VARCHAR(32) NOT NULL"
							");"
		;

		iResult = sqlite3_exec(m_pDatabase, szSql, nullptr, nullptr, &szErrMsg);

		if (iResult != SQLITE_OK)
		{
			std::cout << "화이트리스트 테이블을 생성할 수 없습니다.\n";
			std::cout << "Error(" << iResult << "): " << szErrMsg << "\n";

			sqlite3_free(szErrMsg);
			return false;
		}
	}

	{
		const char* szSql = "SELECT Address FROM whitelist;";

		iResult = sqlite3_exec(m_pDatabase, szSql, CWhiteList::OnWhiteListLoad, nullptr, &szErrMsg);

		if (iResult != SQLITE_OK)
		{
			std::cout << "화이트리스트 테이블을 생성할 수 없습니다.\n";
			std::cout << "Error(" << iResult << "): " << szErrMsg << "\n";

			sqlite3_free(szErrMsg);
			return false;
		}
	}

	std::cout << "성공적으로 화이트리스트를 불러왔습니다.\n";
	return true;
}

bool CWhiteList::Add(std::string strIp)
{
	if (IsIpCanConnect(strIp))
	{
		return false;
	}

	char szSql[64];
	char* szErrMsg;
	int iResult;

	sprintf(szSql, "INSERT INTO whitelist (Address) VALUES ('%s')", strIp.c_str());
	iResult = sqlite3_exec(m_pDatabase, szSql, CWhiteList::OnIpAdd, nullptr, &szErrMsg);

	if (iResult == SQLITE_OK)
	{
		sqlite3_free(szErrMsg);
		m_acceptList.emplace(strIp, true);
		return true;
	}
	
	std::cout << "[SQLite3] Error(" << iResult << "): " << szErrMsg << "\n";
	sqlite3_free(szErrMsg);
	return false;
}

bool CWhiteList::Remove(std::string strIp)
{
	if (!IsIpCanConnect(strIp))
	{
		return false;
	}

	char szSql[64];
	char* szErrMsg;
	int iResult;

	sprintf(szSql, "DELETE FROM whitelist WHERE Address = '%s'", strIp.c_str());
	iResult = sqlite3_exec(m_pDatabase, szSql, CWhiteList::OnIpRemove, nullptr, &szErrMsg);

	if (iResult == SQLITE_OK)
	{
		sqlite3_free(szErrMsg);
		m_acceptList.erase(strIp);
		return true;
	}

	std::cout << "[SQLite3] Error(" << iResult << "): " << szErrMsg << "\n";
	sqlite3_free(szErrMsg);
	return false;
}

int CWhiteList::OnWhiteListLoad(void* pNotUsed, int argc, char** argv, char** aszColumnName)
{
	for (int i = 0; i < argc; ++i)
	{
		assert(argv[i]);

		CWhiteList::GetInstance()->m_acceptList.emplace(std::string(argv[i]), true);

		std::cout << "허용 IP: " << argv[i] << "\n";
	}

	return SQLITE_OK;
}

int CWhiteList::OnIpAdd(void* pNotUsed, int argc, char** argv, char** aszColumnName)
{
	return SQLITE_OK;
}

int CWhiteList::OnIpRemove(void* pNotUsed, int argc, char** argv, char** aszColumnName)
{
	return SQLITE_OK;
}

bool CWhiteList::IsClientCanConnect(SocketAddress* pClient)
{
	return IsIpCanConnect(pClient->host().toString());
}

bool CWhiteList::IsIpCanConnect(std::string& strIp)
{
	auto iter = m_acceptList.find(strIp);
	return (iter != m_acceptList.end());
}
