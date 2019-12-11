#include "StdInc.h"
#include "CPacketTranslator.h"

template <>
CPacketHandler* CSingleton<CPacketHandler>::m_pInstance = nullptr;

CPacketHandler::CPacketHandler(CServer* pServer)
{
	assert(pServer);
	m_pServer = pServer;

	m_pPacketTranslator = new CPacketTranslator(this);
	assert(m_pPacketTranslator);
}

void CPacketHandler::ProcessPacket(StreamSocket* pClient, FIFOBuffer* pBuffer, ePacketType packetType)
{
	CPacket* pPacket = m_pPacketTranslator->Translate(packetType, pClient, pBuffer);

	if (pPacket)
	{
		m_pServer->ProcessPacket(pPacket);

		delete pPacket;
		pPacket = nullptr;
	}
}

