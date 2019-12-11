#pragma once

#define MAX_TIMEOUT 8

typedef void (*pfnPacketHandler)(StreamSocket*, FIFOBuffer*, ePacketType);
typedef bool (*pfnConnectHandler)(StreamSocket*);

// 기본 구조는 클라이언트
class CNetwork
{
protected:
	Poco::Net::Socket* m_pSocket;
	std::vector<pfnPacketHandler> m_packetHandlerList;
	Poco::Thread* m_pThread;
	bool m_bCloseRequested;

public:
	CNetwork();
	~CNetwork();

	bool TryClientStart(const char* szServerAddress, int iServerPort);
	bool Close();

	void StartUpdateThread();

	void AddPacketHandler(pfnPacketHandler handlerFunction);
	void RemovePacketHandler(pfnPacketHandler handlerFunction);

	bool IsInitialized()
	{
		return (m_pSocket != nullptr);
	}

	bool IsRunning()
	{
		return (m_pThread != nullptr);
	}

	Poco::Net::Socket* GetSocket()
	{
		return m_pSocket;
	}

	virtual bool IsServer() { return false; }

protected:
	virtual void OnUpdate();

	size_t GetListIndexOfPacketHandler(pfnPacketHandler handlerFunction);
	void Exit();
};

// 클라이언트 구조를 이어 받아서 서버로 확장
class CNetworkServer : public CNetwork
{
private:
	pfnConnectHandler m_pConnectHandler;

public:
	bool TryStart();

	void RegisterSocketConnectHandler(pfnConnectHandler handlerFunction);

	bool IsServer() { return true; }

private:
	void OnUpdate();

	size_t GetListIndexOfConnectHandler(pfnConnectHandler handlerFunction);
};
