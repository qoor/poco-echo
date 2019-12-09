#include "StdInc.h"
#include "CActionManager.h"

CClient* CSingleton<CClient>::m_pInstance = nullptr;

CClient::CClient(const char* szServerAddress, int iServerPort) : m_pStreamSocket(nullptr)
{
	assert(szServerAddress);
	assert(iServerPort > 0 && iServerPort <= 65535);

	m_pSocketAddress = new SocketAddress(szServerAddress, iServerPort);
	m_pActionManager = new CActionManager(this);
}

CClient::~CClient()
{
	Disconnect();

	if (m_pSocketAddress)
	{
		delete m_pSocketAddress;
		m_pSocketAddress = nullptr;
	}
}

bool CClient::TryConnect()
{
	if (m_pStreamSocket)
	{
		return false;
	}

	assert(m_pSocketAddress);

	m_pStreamSocket = new StreamSocket();
	assert(m_pStreamSocket);

	try
	{
		m_pStreamSocket->connectNB(*m_pSocketAddress);
	}
	catch (Exception e)
	{
		Disconnect();

		std::cout << "서버에 접속할 수 없습니다: " << e.displayText() << std::endl;
		return false;
	}

	return true;
}

bool CClient::Disconnect()
{
	if (!m_pStreamSocket)
	{
		return false;
	}

	m_pStreamSocket->close();
	delete m_pStreamSocket;
	m_pStreamSocket = nullptr;

	return true;
}
