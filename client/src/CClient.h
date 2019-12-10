#pragma once

class CClient : public CSingleton<CClient>
{
private:
	SocketAddress* m_pSocketAddress;
	StreamSocket* m_pStreamSocket;
	class CActionManager* m_pActionManager;
	CPacketHandler* m_pPacketHandler;

private:
	CClient();

public:
	CClient(const char* szServerAddress, int iServerPort);
	~CClient();

	bool TryConnect();
	bool Disconnect();
	void Run();
	bool ListenAction();
	bool ListenPacket();
};
