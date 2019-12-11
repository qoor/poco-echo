#include "StdInc.h"
#include "CActionManager.h"

CClient* CSingleton<CClient>::m_pInstance = nullptr;

CClient::CClient() : m_pNetwork(nullptr)
{
	m_pActionManager = new CActionManager(this);
}

CClient::~CClient()
{
	Disconnect();
}

bool CClient::TryConnect(const char* szServerAddress, int iServerPort)
{
	assert(szServerAddress);
	assert(iServerPort > 0 && iServerPort <= 65535);

	m_pNetwork = new CNetwork();
	assert(m_pNetwork);

	std::cout << "���� " << szServerAddress << ":" << iServerPort << "�� ���� ��..\n";

	if (m_pNetwork->TryClientStart(szServerAddress, iServerPort))
	{
		m_pNetwork->AddPacketHandler(CClient::PacketHandler);
	}
	else
	{
		std::cout << "������ ������ �� �����ϴ�.\n";
	}

	return true;
}

bool CClient::Disconnect()
{
	if (!m_pNetwork)
	{
		return false;
	}

	if (m_pNetwork->IsInitialized())
	{
		m_pNetwork->Close();
	}

	delete m_pNetwork;
	m_pNetwork = nullptr;
	return true;
}

void CClient::Run()
{
	if (!m_pNetwork || !m_pNetwork->IsInitialized())
	{
		return;
	}

	m_pNetwork->StartUpdateThread();

	Poco::Thread actionThread;

	actionThread.start(*m_pActionManager);
	actionThread.join();
}

bool CClient::ListenAction()
{
	int iActionType;
	void* pResult;
	int iResultSize;

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
		FIFOBuffer buffer(MAX_BUFFER_LENGTH);
		StreamSocket* pSocket = dynamic_cast<StreamSocket*>(m_pNetwork->GetSocket());

		++iResultSize; // EOS ����

		buffer.write(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));
		buffer.write(reinterpret_cast<char*>(&iResultSize), sizeof(int));
		buffer.write(reinterpret_cast<char*>(pResult), iResultSize + 1);

		while (!pSocket->poll(timeOut, Poco::Net::Socket::SELECT_WRITE));

		int nCount = pSocket->sendBytes(buffer);

		if (nCount == 0)
		{
			std::cout << "���� ����\n";
		}
		else
		{
			std::cout << "�������� �޼����� �����Ͽ����ϴ�.\n";
		}
	}

	return true;
}

void CClient::PacketHandler(StreamSocket* pSocket, FIFOBuffer* pBuffer, ePacketType packetType)
{
	switch (packetType)
	{
		case PACKET_TYPE_ACCEPT:
		{
			std::cout << "���� " << CClient::GetInstance()->m_pNetwork->GetSocket()->address().toString() << "�� ���� ����!\n";
		}
		case PACKET_TYPE_MESSAGE:
		{
			int iLength;
			char* szMessage;

			pBuffer->read(reinterpret_cast<char*>(&iLength), sizeof(int));

			szMessage = new char[iLength];
			pBuffer->read(szMessage, iLength);

			std::cout << "�������Լ� ���� �޼���: " << szMessage << "\n";
			break;
		}

		default:
		{
			std::cout << "�������Լ� �� �� ���� ��Ŷ�� ���Խ��ϴ�. (id: " << packetType << "\n";
		}
	}
}
