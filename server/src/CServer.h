#pragma once

#define SERVER_PORT 28210

class CServer : public CSingleton<CServer>
{
private:
	CWhiteList* m_pWhiteList;
	CPacketHandler* m_pPacketHandler;
	CActionManager* m_pActionManager;
	CNetwork* m_pNetwork;


public:
	CServer(CWhiteList* pWhiteList);
	~CServer();

	bool TryStart();
	void Run();
	bool Close();

	static void PacketHandler(StreamSocket* pClientSocket, FIFOBuffer* pBuffer, ePacketType packetType);
};
