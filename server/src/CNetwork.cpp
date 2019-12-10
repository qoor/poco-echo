#include "StdInc.h"

CNetwork::CNetwork()
{
	m_pSocket = nullptr;
	m_pThread = nullptr;
	m_bIsServer = false;
	m_bCloseRequested = false;
}

CNetwork::~CNetwork()
{
	if (m_pSocket)
	{
		Close();
		m_pThread->join();
	}
}

bool CNetwork::TryStartServer()
{
	if (m_pSocket)
	{
		return false;
	}

	try
	{
		m_bIsServer = true;

		m_pSocket = new ServerSocket(SocketAddress(DEFAULT_SERVER_PORT));
		assert(m_pSocket);

		StartUpdateThread();
	}
	catch (Exception e)
	{
		if (m_pThread)
		{
			delete m_pThread;
		}

		Close();

		std::cout << "서버 초기화를 실패했습니다: " << e.displayText() << "\n";
		return false;
	}

	return true;
}

bool CNetwork::Close()
{
	if (!m_pSocket)
	{
		return false;
	}

	m_bCloseRequested = true;
	return true;
}

void CNetwork::StartUpdateThread()
{
	m_pThread = new Poco::Thread();
	assert(m_pThread);

	m_pThread->startFunc(CNetwork::OnUpdate);
}

void CNetwork::OnUpdate()
{
	if (m_bIsServer)
	{
		ServerSocket* pServerSocket = dynamic_cast<ServerSocket*>(m_pSocket);
		Poco::Net::Socket::SocketList clients;
		Poco::Timespan timeOut(1);
		int iCount;
		ePacketType packetType;
		StreamSocket* pClientSocket;

		clients.push_back(*pServerSocket);

		while (true)
		{
			if (m_bCloseRequested)
			{
				break;
			}

			Poco::Net::Socket::SocketList readList(clients.begin(), clients.end());
			Poco::Net::Socket::SocketList writeList(clients.begin(), clients.end());
			Poco::Net::Socket::SocketList exceptList(clients.begin(), clients.end());

			iCount = Poco::Net::Socket::select(readList, writeList, exceptList, timeOut);

			if (iCount == 0)
			{
				continue;
			}

			Poco::Net::Socket::SocketList dropList;

			for (auto& readSocket : readList)
			{
				if (readSocket == *pServerSocket)
				{
					auto client = pServerSocket->acceptConnection();

					clients.push_back(client);
					continue;
				}
				
				Poco::FIFOBuffer buffer(MAX_BUFFER_LENGTH);

				pClientSocket = dynamic_cast<StreamSocket*>(&readSocket);
				iCount = pClientSocket->receiveBytes(buffer);

				if (iCount == 0)
				{
					dropList.push_back(readSocket);
					continue;
				}

				buffer.read(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));

				Poco::Mutex& bufferMutex = buffer.mutex();

				bufferMutex.tryLock();

				for (auto handlerFunction : m_handlerList)
				{
					
					handlerFunction(pClientSocket, &buffer, packetType);
				}

				bufferMutex.unlock();
			}
		}

		Exit();
	}
}

void CNetwork::Exit()
{
	assert(m_pSocket);

	m_pSocket->close();

	delete m_pSocket;
	m_pSocket = nullptr;

	m_bCloseRequested = false;

	delete m_pThread;
	m_pThread = nullptr;
}

void CNetwork::AddPacketHandler(pPacketHandler handlerFunction)
{
	if (GetListIndexOfHandler(handlerFunction) != -1)
	{
		return;
	}

	m_handlerList.push_back(handlerFunction);
}

void CNetwork::RemovePacketHandler(pPacketHandler handlerFunction)
{
	int iIndex;

	if ((iIndex = GetListIndexOfHandler(handlerFunction)) == -1)
	{
		return;
	}

	m_handlerList.erase(m_handlerList.begin() + iIndex);
}

size_t CNetwork::GetListIndexOfHandler(pPacketHandler handlerFunction)
{
	for (int i = 0; i < m_handlerList.size(); ++i)
	{
		if (m_handlerList[i] == handlerFunction)
		{
			return i;
		}
	}

	return -1;
}
