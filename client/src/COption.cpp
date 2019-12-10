#include "StdInc.h"
#include "COption.h"

COption* CSingleton<COption>::m_pInstance = nullptr;

COption::COption() : m_szServerAddress(nullptr), m_iServerPort(DEFAULT_SERVER_PORT)
{

}

COption::~COption()
{
	if (m_szServerAddress)
	{
		delete m_szServerAddress;
		m_szServerAddress = nullptr;
	}
}

bool COption::ParseOptions(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cout << "사용법: echo-client server_ip [server_port = 28210]\n";
		return false;
	}

	if (argc >= 3)
	{
		bool bInvalidPort = false;

		for (int i = 0; argv[2][i] != '\0'; ++i)
		{
			if (!isdigit(argv[2][i]))
			{
				bInvalidPort = true;
				break;
			}
		}

		if (!bInvalidPort)
		{
			int iPort = atoi(argv[2]);

			if (iPort <= 0 || iPort > 65535)
			{
				bInvalidPort = true;
			}
		}

		if (bInvalidPort)
		{
			std::cout << "올바르지 않은 포트 번호입니다.\n";
			return false;
		}
	}

	size_t addressLength = strlen(argv[1]);

	m_szServerAddress = new char[addressLength + 1];
	strncpy(m_szServerAddress, argv[1], addressLength);
	m_szServerAddress[addressLength] = '\0';

	return true;
}
