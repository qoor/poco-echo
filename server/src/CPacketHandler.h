#pragma once

class CPacketHandler : public CSingleton<CPacketHandler>
{
private:
	class CServer* m_pServer;
	class CPacketTranslator* m_pPacketTranslator;

public:
	CPacketHandler(CServer* pServer);

	void ProcessPacket(StreamSocket* pClient, FIFOBuffer* pBuffer, ePacketType packetType);
};
