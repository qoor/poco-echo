#pragma once

class CClient : public CSingleton<CClient>
{
private:
	class CActionManager* m_pActionManager;
	CPacketHandler* m_pPacketHandler;
	CNetwork* m_pNetwork;

public:
	CClient();
	~CClient();

	bool TryConnect(const char* szServerAddress, int iServerPort);
	bool Disconnect();
	void Run();

	bool ListenAction();

	static void PacketHandler(StreamSocket* pSocket, FIFOBuffer* pBuffer, ePacketType packetType);
};
