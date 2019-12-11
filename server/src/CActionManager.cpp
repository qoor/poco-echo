#include "StdInc.h"
#include "CActionManager.h"

template <>
CActionManager* CSingleton<CActionManager>::m_pInstance = nullptr;

CActionManager::CActionManager(CWhiteList* pWhiteList)
{
	assert(pWhiteList);
	m_pWhiteList = pWhiteList;
}

void CActionManager::ShowHelp()
{
	std::cout << "====================================\n";
	std::cout << "\t\t\t��� ���\n";
	std::cout << "1: ȭ��Ʈ����Ʈ IP �߰�\n";
	std::cout << "2: ȭ��Ʈ����Ʈ IP ����\n";
	std::cout << "3: ����\n";
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
			char szIp[16];

			std::cout << "����� IP �ּҸ� �Է��� �ֽʽÿ�: ";

			std::cin >> szIp;

			if (!m_pWhiteList->Add(szIp))
			{
				std::cout << "IP ����� �����߽��ϴ�.\n";
				return false;
			}

			break;
		}

		case 2:
		{
			char szIp[16];

			std::cout << "������ IP �ּҸ� �Է��� �ֽʽÿ�: ";

			std::cin >> szIp;

			if (!m_pWhiteList->Remove(szIp))
			{
				std::cout << "��ϵ��� ���� IP�̰ų� ���Ÿ� �����߽��ϴ�.\n";
				return false;
			}

			break;
		}

		case 3:
		{
			std::cout << "������ �����մϴ�.\n";
			exit(0);
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

void CActionManager::run()
{
	while (true)
	{
		ShowHelp();
	}
}
