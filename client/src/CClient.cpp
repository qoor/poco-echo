#include "StdInc.h"
#include "CActionManager.h"

#define MAX_TIMEOUT 8

CClient* CSingleton<CClient>::m_pInstance = nullptr;

CClient::CClient(const char* szServerAddress, int iServerPort) : m_pStreamSocket(nullptr)
{
	assert(szServerAddress);
	assert(iServerPort > 0 && iServerPort <= 65535);

	m_pSocketAddress = new SocketAddress(szServerAddress, iServerPort);
	assert(m_pSocketAddress);

	m_pActionManager = new CActionManager(this);
	m_pPacketHandler = new CPacketHandler();
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
		Poco::Timespan timeOut(1, 0);

		m_pStreamSocket->connectNB(*m_pSocketAddress);

		std::cout << "서버 " << m_pSocketAddress->toString() << "에 접속 하는 중..\n";
		m_pStreamSocket->poll(Poco::Timespan(MAX_TIMEOUT, 0), Poco::Net::Socket::SELECT_READ);

		std::cout << "인증 확인..\n";

		ePacketType packetType;
		int nCount = m_pStreamSocket->receiveBytes(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));

		if (nCount == 0)
		{
			throw Exception("Server closed the connection.");
		}
		else
		{
			if (packetType != PACKET_TYPE_ALLOW)
			{
				throw Exception("Access denied.");
			}
		}

		std::cout << "서버 " << m_pSocketAddress->toString() << " 접속 성공!\n";
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

void CClient::Run()
{
	if (!m_pStreamSocket)
	{
		return;
	}

	while (true)
	{
		ListenAction();

		if (!ListenPacket())
		{
			break;
		}
	}
}

bool CClient::ListenAction()
{
	int iActionType;
	void* pResult;
	int iResultSize;

	m_pActionManager->ShowHelp();

	if (m_pActionManager->PopActionResult(&iActionType, &pResult, &iResultSize))
	{
		ePacketType packetType;

		switch (iActionType)
		{
			case 1:
			{
				packetType = PACKET_TYPE_MESSAGE;
				break;
			}

			default:
			{
				return false;
			}
		}

		Poco::Timespan timeOut(1, 0);

		while (!m_pStreamSocket->poll(timeOut, Poco::Net::Socket::SELECT_WRITE));
		m_pStreamSocket->sendBytes(&iResultSize, sizeof(int));
		
		while (!m_pStreamSocket->poll(timeOut, Poco::Net::Socket::SELECT_WRITE));
		m_pStreamSocket->sendBytes(pResult, iResultSize);

		while (!m_pStreamSocket->poll(timeOut, Poco::Net::Socket::SELECT_WRITE));

		m_pStreamSocket->sendBytes(&packetType, sizeof(ePacketType));
	}

	return true;
}

bool CClient::ListenPacket()
{
	Poco::Timespan timeOut(1, 0);
	int iPacketType;
	int nByteCount;

	while (!m_pStreamSocket->poll(timeOut, Poco::Net::Socket::SELECT_READ));

	nByteCount = m_pStreamSocket->receiveBytes(&nByteCount, sizeof(int));

	if (nByteCount == 0)
	{
		return false;
	}

	while (!m_pStreamSocket->poll(timeOut, Poco::Net::Socket::SELECT_READ));

	Poco::FIFOBuffer buffer(nByteCount + 1);

	//nByteCount = pSocket->receiveBytes(&iPacketType, sizeof(int));
	nByteCount = m_pStreamSocket->receiveBytes(buffer);

	if (nByteCount == 0)
	{
		return false;
	}

	iPacketType = *(reinterpret_cast<int*>(buffer.next()));

	if (!m_pPacketHandler->ProcessPacket(&buffer, static_cast<ePacketType>(iPacketType)))
	{
		return false;
	}

	return true;
}
