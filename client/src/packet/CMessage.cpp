#include "StdInc.h"
#include "CMessage.h"

bool CMessage::ShowMessage(FIFOBuffer* pBuffer)
{
	if (pBuffer->isEmpty())
	{
		return false;
	}

	char* szMessage = pBuffer->next();

	std::cout << "�������Լ� ���� �޼���: " << szMessage << "\n";
	return true;
}
