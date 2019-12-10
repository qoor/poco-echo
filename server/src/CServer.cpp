#include "StdInc.h"

CServer* CSingleton<CServer>::m_pInstance = nullptr;

CServer::CServer(CWhiteList* pWhiteList) : m_pWhiteList(pWhiteList), m_pNetwork(nullptr)
{
	assert(pWhiteList);

	m_pPacketHandler = new CPacketHandler();
	assert(m_pPacketHandler);

	m_pActionManager = new CActionManager(pWhiteList);
}

CServer::~CServer()
{
	Close();
}

bool CServer::TryStart()
{
	m_pNetwork = new CNetwork();
	assert(m_pNetwork);

	if (m_pNetwork->TryStartServer())
	{
		m_pNetwork->AddPacketHandler(CServer::PacketHandler);
		return true;
	}
}

void CServer::Run()
{
	while (true)
	{
		m_pActionManager->ShowHelp();
	}
}

void CServer::PacketHandler(StreamSocket* pClientSocket, FIFOBuffer* pBuffer, ePacketType packetType)
{
	CServer* pServer = CServer::GetInstance();
	ePacketType sendPacketType;

	switch (packetType)
	{
		case PACKET_TYPE_REQUEST_JOIN:
		{
			if (!pServer->m_pWhiteList->IsClientCanConnect(&pClientSocket->address()))
			{
				sendPacketType = PACKET_TYPE_KICK;

				pClientSocket->sendBytes(reinterpret_cast<char*>(&sendPacketType), sizeof(ePacketType));
				pClientSocket->close();
			}

			break;
		}

		default:
		{
			
		}
	}
}

bool CServer::Close()
{
	if (m_pNetwork)
	{
		if (m_pNetwork->IsRunning())
		{
			m_pNetwork->Close();
		}

		delete m_pNetwork;
		m_pNetwork = nullptr;
		return true;
	}

	return false;
}
