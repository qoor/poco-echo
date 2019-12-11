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
	std::cout << "\t\t\t기능 목록\n";
	std::cout << "1: 화이트리스트 IP 추가\n";
	std::cout << "2: 화이트리스트 IP 제거\n";
	std::cout << "3: 종료\n";
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
			char szIp[16];

			std::cout << "등록할 IP 주소를 입력해 주십시오: ";

			std::cin >> szIp;

			if (!m_pWhiteList->Add(szIp))
			{
				std::cout << "IP 등록을 실패했습니다.\n";
				return false;
			}

			break;
		}

		case 2:
		{
			char szIp[16];

			std::cout << "제거할 IP 주소를 입력해 주십시오: ";

			std::cin >> szIp;

			if (!m_pWhiteList->Remove(szIp))
			{
				std::cout << "등록되지 않은 IP이거나 제거를 실패했습니다.\n";
				return false;
			}

			break;
		}

		case 3:
		{
			std::cout << "서버를 종료합니다.\n";
			exit(0);
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

void CActionManager::run()
{
	while (true)
	{
		ShowHelp();
	}
}
