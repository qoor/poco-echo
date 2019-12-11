#pragma once

class CPacketTranslator : public CSingleton<CPacketTranslator>
{
private:
	CPacketHandler* m_pPacketHandler;

public:
	CPacketTranslator(CPacketHandler* pPacketManager);

	CPacket* Translate(ePacketType packetType, StreamSocket* pClient, FIFOBuffer* pBuffer);
};
