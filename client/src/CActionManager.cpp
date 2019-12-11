#include "StdInc.h"
#include "CActionManager.h"

CActionManager* CSingleton<CActionManager>::m_pInstance = nullptr;

CActionManager::CActionManager(CClient* pClient) : m_pActionResult(nullptr), m_actionResultSize(0), m_iActionType(0)
{
	assert(pClient);
	m_pClient = pClient;
}

void CActionManager::ShowHelp()
{
	std::cout << "====================================\n";
	std::cout << "\t\t\t��� ���\n";
	std::cout << "1: �޼��� ����\n";
	std::cout << "2: ����\n";
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
			int iMessageLength;

			std::cout << '\n';
			std::cout << "���� �� �޼����� �Է��� �ֽʽÿ� (�ִ� 256����): ";

			std::cin >> szMessage;
			iMessageLength = strlen(szMessage);

			if (iMessageLength == 0)
			{
				GetAction(1);
			}

			m_pActionResult = new char[iMessageLength + 1];
			strncpy(reinterpret_cast<char*>(m_pActionResult), szMessage, iMessageLength);
			reinterpret_cast<char*>(m_pActionResult)[iMessageLength] = '\0';
			m_actionResultSize = iMessageLength + 1;
			break;
		}

		case 2:
		{
			std::cout << "Ŭ���̾�Ʈ�� �����մϴ�.\n";
			exit(0);
			break;
		}

		default:
		{
			std::cout << "�߸��� ��� ��ȣ�Դϴ�. �ٽ� �Է��� �ֽʽÿ�.\n";
			return false;
		}
	}

	m_iActionType = iAction;
	m_pClient->ListenAction();

	return true;
}

bool CActionManager::PopActionResult(int* piDestActionType, void** ppDestination, int* piDestSize)
{
	assert(ppDestination);
	assert(piDestSize);

	*piDestActionType = 0;
	*ppDestination = nullptr;
	*piDestSize = 0;

	std::cout << "pop result 0\n";

	if (m_pActionResult == nullptr || m_actionResultSize == 0)
	{
		return false;
	}

	std::cout << "pop result 1\n";

	*piDestActionType = m_iActionType;
	*ppDestination = m_pActionResult;
	*piDestSize = m_actionResultSize;

	m_iActionType = 0;
	m_pActionResult = nullptr;
	m_actionResultSize = 0;

	std::cout << "pop result 2\n";
	return true;
}

void CActionManager::run()
{
	while (true)
	{
		ShowHelp();
	}
}
