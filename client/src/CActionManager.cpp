#include "StdInc.h"
#include "CActionManager.h"

CActionManager* CSingleton<CActionManager>::m_pInstance = nullptr;

CActionManager::CActionManager(CClient* pClient)
{
	assert(pClient);
	m_pClient = pClient;
}

void CActionManager::ShowHelp()
{
	std::cout << "====================================\n";
	std::cout << "\t\t\t기능 목록\n";
	std::cout << "1: 메세지 전송\n";
	std::cout << "====================================\n";
	std::cout << '\n';
	std::cout << "기능을 선택해 주십시오: ";

	int iAction;

	std::cin >> iAction;

	if (!GetAction(iAction))
	{
		ShowHelp();
	}
}

bool CActionManager::GetAction(int iAction)
{
	switch (iAction)
	{
		case 1:
		{
			char szMessage[256];

			std::cout << '\n';
			std::cout << "전송 할 메세지를 입력해 주십시오 (최대 256글자): ";

			std::cin >> szMessage;

			if (strlen(szMessage) == 0)
			{
				GetAction(1);
			}

			break;
		}

		default:
		{
			std::cout << "잘못된 기능 번호입니다. 다시 입력해 주십시오.\n";
			return false;
		}
	}

	return true;
}
