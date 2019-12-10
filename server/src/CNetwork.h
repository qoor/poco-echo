#pragma once

#include <Poco/Thread.h>

typedef void (*pPacketHandler)(StreamSocket*, FIFOBuffer*, ePacketType);

class CNetwork
{
private:
	Poco::Net::Socket* m_pSocket;
	std::vector<pPacketHandler> m_handlerList;
	Poco::Thread* m_pThread;
	bool m_bIsServer;
	bool m_bCloseRequested;

public:
	CNetwork();
	~CNetwork();

	bool TryStartServer();
	bool TryStartClient(const char* szServerAddress, int iServerPort);
	bool Close();

	void StartUpdateThread();
	
	void OnUpdate();

	void AddPacketHandler(pPacketHandler handlerFunction);
	void RemovePacketHandler(pPacketHandler handlerFunction);

	bool IsRunning()
	{
		return (m_pSocket != nullptr);
	}

	Poco::Net::Socket* GetSocket()
	{
		return m_pSocket;
	}

private:
	size_t GetListIndexOfHandler(pPacketHandler handlerFunction);
	void Exit();
};
