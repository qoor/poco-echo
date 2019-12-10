#include "StdInc.h"
#include "main.h"

int main()
{
	CWhiteList* pWhiteList = new CWhiteList();
	assert(pWhiteList);

	pWhiteList->Initialize();

	if (!pWhiteList->TryGetList())
	{
		delete pWhiteList;
		return 0;
	}

	CServer* pServer = new CServer(pWhiteList);
	assert(pServer);

	if (pServer->TryStart())
	{
		pServer->Run();

		pServer->Close();
	}

	delete pWhiteList;
	delete pServer;
	return 0;
}
