#pragma once

#define DEFAULT_SERVER_PORT 28210

class CClient : public CSingleton<CClient>
{
private:
	SocketAddress* m_pSocketAddress;
	StreamSocket* m_pStreamSocket;
	class CActionManager* m_pActionManager;

private:
	CClient();

public:
	CClient(const char* szServerAddress, int iServerPort);
	~CClient();

	bool TryConnect();
	bool Disconnect();
};
