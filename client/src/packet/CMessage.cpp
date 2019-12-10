#include "StdInc.h"
#include "CMessage.h"

bool CMessage::ShowMessage(FIFOBuffer* pBuffer)
{
	if (pBuffer->isEmpty())
	{
		return false;
	}

	char* szMessage = pBuffer->next();

	std::cout << "서버에게서 받은 메세지: " << szMessage << "\n";
	return true;
}
