#include "StdInc.h"
#include "main.h"
#include "COption.h"

int main(int argc, char** argv)
{
	COption* pOption = new COption();
	assert(pOption);

	if (!pOption->ParseOptions(argc, argv))
	{
		return 0;
	}

	CClient* pClient = new CClient();
	assert(pClient);

	if (pClient->TryConnect(pOption->GetServerAddress(), pOption->GetServerPort()))
	{
		pClient->Run();

		pClient->Disconnect();
	}

	delete pOption;
	delete pClient;

	return 0;
}
