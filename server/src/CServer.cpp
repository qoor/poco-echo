#include "StdInc.h"

template <>
CServer* CSingleton<CServer>::m_pInstance = nullptr;

CServer::CServer(CWhiteList* pWhiteList) : m_pWhiteList(pWhiteList), m_pNetwork(nullptr)
{
	assert(pWhiteList);

	m_pPacketHandler = new CPacketHandler(this);
	assert(m_pPacketHandler);

	m_pActionManager = new CActionManager(pWhiteList);
}

CServer::~CServer()
{
	Close();
}

bool CServer::TryStart()
{
	m_pNetwork = new CNetworkServer();
	assert(m_pNetwork);

	if (m_pNetwork->TryStart())
	{
		m_pNetwork->AddPacketHandler(CServer::PacketHandler);
		m_pNetwork->RegisterSocketConnectHandler(CServer::ConnectHandler);
		return true;
	}
	
	std::cout << "���� �ʱ�ȭ�� �����߽��ϴ�.\n";
	return false;
}

void CServer::Run()
{
	if (!m_pNetwork || !m_pNetwork->IsInitialized())
	{
		return;
	}

	m_pNetwork->StartUpdateThread();

	Poco::Thread actionThread;
	CActionManager& actionManager = *m_pActionManager;

	actionThread.start(actionManager);
	actionThread.join();
}

void CServer::PacketHandler(StreamSocket* pClientSocket, FIFOBuffer* pBuffer, ePacketType packetType)
{
	CServer* pServer = CServer::GetInstance();

	std::cout << "��Ŷ ����\n";
	
	pServer->m_pPacketHandler->ProcessPacket(pClientSocket, pBuffer, packetType);
}

void CServer::ProcessPacket(CPacket* pPacket)
{
	if (!pPacket)
	{
		return;
	}

	switch (pPacket->GetPacketType())
	{
		case PACKET_TYPE_MESSAGE:
		{
			Packet_Message(dynamic_cast<CMessagePacket*>(pPacket));
			break;
		}
	}
}

void CServer::Packet_Message(CMessagePacket* pPacket)
{
	StreamSocket* pClient = pPacket->GetSocket();
	FIFOBuffer buffer(MAX_BUFFER_LENGTH);
	char* szMessage = pPacket->m_szMessage;

	assert(szMessage);
	std::cout << "Ŭ���̾�Ʈ (" << pClient->address().toString() << ")(��)�� ���� �޼��� ����: " << szMessage << "\n";

	CMessageSendPacket msgSendPacket(szMessage);

	msgSendPacket.Write(&buffer);
	pClient->sendBytes(buffer);
}

bool CServer::ConnectHandler(StreamSocket* pClientSocket)
{
	CServer* pServer = CServer::GetInstance();
	SocketAddress clientAddress(pClientSocket->peerAddress());

	std::cout << "Ŭ���̾�Ʈ " << pClientSocket->peerAddress().host().toString() << " ���� ���..\n";

	if (!pServer->m_pWhiteList->IsClientCanConnect(clientAddress))
	{
		std::cout << "�������� ���� Ŭ���̾�Ʈ\n";
		return false;
	}

	std::cout << "���� ����!\n";
	return true;
}

bool CServer::Close()
{
	if (m_pNetwork)
	{
		if (m_pNetwork->IsInitialized())
		{
			m_pNetwork->Close();
		}

		delete m_pNetwork;
		m_pNetwork = nullptr;
		return true;
	}

	return false;
}
