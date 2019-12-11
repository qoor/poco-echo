#include <iostream>
#include <functional>
#include <cassert>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/FIFOBufferStream.h>

using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Exception;
using Poco::FIFOBuffer;
using Poco::FIFOBufferStream;

#include "Packets.h"
#include "CNetwork.h"

CNetwork::CNetwork()
{
	m_pSocket = nullptr;
	m_pThread = nullptr;
	m_bCloseRequested = false;
}

CNetwork::~CNetwork()
{
	if (m_pSocket)
	{
		Close();
	}
}

bool CNetwork::TryClientStart(const char* szServerAddress, int iServerPort)
{
	if (IsServer() || m_pSocket)
	{
		return false;
	}

	m_pSocket = new StreamSocket();
	assert(m_pSocket);

	try
	{
		StreamSocket* pSocket = reinterpret_cast<StreamSocket*>(m_pSocket);
		Poco::Timespan timeOut(1, 0);

		pSocket->connectNB(SocketAddress(szServerAddress, iServerPort));
		pSocket->poll(Poco::Timespan(MAX_TIMEOUT, 0), Poco::Net::Socket::SELECT_READ);

		ePacketType packetType;
		int nCount = pSocket->receiveBytes(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));

		if (nCount == 0)
		{
			throw Exception("Server closed the connection.");
		}
		else
		{
			if (packetType != PACKET_TYPE_ACCEPT)
			{
				throw Exception("Access denied.");
			}
		}
	}
	catch (Exception e)
	{
		if (m_pThread)
		{
			delete m_pThread;
		}

		Close();
		return false;
	}

	return true;
}

void CNetwork::OnUpdate()
{
	assert(m_pSocket);
	assert(!IsServer());

	Poco::Timespan timeOut(1, 0);
	int nByteCount;
	StreamSocket* pSocket = reinterpret_cast<StreamSocket*>(m_pSocket);
	ePacketType packetType;

	while (true)
	{
		try
		{
			// 패킷 타입 읽기
			while (!m_bCloseRequested && !pSocket->poll(timeOut, Poco::Net::Socket::SELECT_READ));

			if (m_bCloseRequested)
			{
				break;
			}

			nByteCount = pSocket->receiveBytes(&packetType, sizeof(ePacketType));

			if (nByteCount == 0)
			{
				break;
			}

			// 데이터 읽기

			FIFOBuffer buffer(MAX_BUFFER_LENGTH);

			nByteCount = pSocket->receiveBytes(buffer);

			if (nByteCount == 0)
			{
				break;
			}


			// 핸들러에 데이터 전달
			size_t bufferSize = buffer.size();
			Poco::Mutex& bufferMutex = buffer.mutex();

			bufferMutex.lock();

			for (auto handlerFunction : m_packetHandlerList)
			{
				handlerFunction(pSocket, &buffer, static_cast<ePacketType>(packetType));
			}

			bufferMutex.unlock();
		}
		catch (Exception)
		{
			std::cout << "서버와의 연결이 끊어졌습니다.\n";
			Exit();
		}
	}

	Exit();
}

bool CNetwork::Close()
{
	if (!m_pSocket)
	{
		return false;
	}

	if (m_pThread)
	{
		m_bCloseRequested = true;

		m_pThread->join();
	}
	else
	{
		Exit();
	}

	return true;
}

void CNetwork::StartUpdateThread()
{
	m_pThread = new Poco::Thread();
	assert(m_pThread);

	auto pmfnFunc = std::bind(&CNetwork::OnUpdate, this);
	m_pThread->startFunc(pmfnFunc);
}

void CNetwork::Exit()
{
	assert(m_pSocket);

	m_pSocket->close();

	delete m_pSocket;
	m_pSocket = nullptr;

	m_bCloseRequested = false;

	if (m_pThread)
	{
		delete m_pThread;
		m_pThread = nullptr;
	}
}

void CNetwork::AddPacketHandler(pfnPacketHandler handlerFunction)
{
	if (GetListIndexOfPacketHandler(handlerFunction) != -1)
	{
		return;
	}

	m_packetHandlerList.push_back(handlerFunction);
}

void CNetwork::RemovePacketHandler(pfnPacketHandler handlerFunction)
{
	int iIndex;

	if ((iIndex = GetListIndexOfPacketHandler(handlerFunction)) == -1)
	{
		return;
	}

	m_packetHandlerList.erase(m_packetHandlerList.begin() + iIndex);
}

size_t CNetwork::GetListIndexOfPacketHandler(pfnPacketHandler handlerFunction)
{
	for (int i = 0; i < m_packetHandlerList.size(); ++i)
	{
		if (m_packetHandlerList[i] == handlerFunction)
		{
			return i;
		}
	}

	return -1;
}

bool CNetworkServer::TryStart()
{
	if (!IsServer() || m_pSocket)
	{
		return false;
	}

	try
	{
		m_pSocket = new ServerSocket(SocketAddress(DEFAULT_SERVER_PORT));
		assert(m_pSocket);

		//StartUpdateThread();
	}
	catch (Exception e)
	{
		if (m_pThread)
		{
			delete m_pThread;
		}

		Close();
		return false;
	}

	return true;
}

void CNetworkServer::OnUpdate()
{
	assert(m_pSocket);
	assert(IsServer());

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

		iCount = pServerSocket->select(readList, writeList, exceptList, timeOut);

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

				if (m_pConnectHandler && !m_pConnectHandler(&client))
				{
					packetType = PACKET_TYPE_KICK;

					client.sendBytes(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));
					client.close();
					continue;
				}

				packetType = PACKET_TYPE_ACCEPT;
				client.sendBytes(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));
				clients.push_back(client);
				continue;
			}

			FIFOBuffer buffer(MAX_BUFFER_LENGTH);

			pClientSocket = static_cast<StreamSocket*>(&readSocket);
			iCount = pClientSocket->receiveBytes(buffer);

			if (iCount == 0)
			{
				dropList.push_back(readSocket);
				continue;
			}

			buffer.read(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));

			Poco::Mutex& bufferMutex = buffer.mutex();

			bufferMutex.lock();

			for (auto handlerFunction : m_packetHandlerList)
			{
				handlerFunction(pClientSocket, &buffer, packetType);
			}

			bufferMutex.unlock();
		}

		for (auto& dropTarget : dropList)
		{
			auto iter = std::find_if(clients.begin(), clients.end(), [&dropTarget](auto& target) { return (dropTarget == target); });

			if (iter != clients.end())
			{
				iter->close();
				clients.erase(iter);
			}
		}
	}

	for (auto& client : clients)
	{
		client.close();
	}

	Exit();
}

void CNetworkServer::RegisterSocketConnectHandler(pfnConnectHandler handlerFunction)
{
	m_pConnectHandler = handlerFunction;
}
