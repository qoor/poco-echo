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
	std::cout << "\t\t\t��� ���\n";
	std::cout << "1: �޼��� ����\n";
	std::cout << "====================================\n";
	std::cout << '\n';
	std::cout << "����� ������ �ֽʽÿ�: ";

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
			std::cout << "���� �� �޼����� �Է��� �ֽʽÿ� (�ִ� 256����): ";

			std::cin >> szMessage;

			if (strlen(szMessage) == 0)
			{
				GetAction(1);
			}

			break;
		}

		default:
		{
			std::cout << "�߸��� ��� ��ȣ�Դϴ�. �ٽ� �Է��� �ֽʽÿ�.\n";
			return false;
		}
	}

	return true;
}
