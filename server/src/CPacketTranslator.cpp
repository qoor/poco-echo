#include "StdInc.h"
#include "CPacketTranslator.h"

template <>
CPacketTranslator* CSingleton<CPacketTranslator>::m_pInstance = nullptr;

CPacketTranslator::CPacketTranslator(CPacketHandler* pPacketHandler)
{
	assert(pPacketHandler);
	m_pPacketHandler = pPacketHandler;
}

CPacket* CPacketTranslator::Translate(ePacketType packetType, StreamSocket* pClient, FIFOBuffer* pBuffer)
{
	assert(pClient);
	assert(pBuffer);

	CPacket* pPacket = nullptr;

	switch (packetType)
	{
		case PACKET_TYPE_MESSAGE:
		{
			pPacket = new CMessagePacket();
			break;
		}

		default:
		{
			std::cout << "Ŭ���̾�Ʈ (" << pClient->address().toString() << ")(��)�� ���� �� �� ���� ��Ŷ�� ���Խ��ϴ�. (id:" << packetType << "\n";
		}
	}

	if (pPacket)
	{
		pPacket->SetSocket(pClient);

		if (!pPacket->Read(pBuffer))
		{
			delete pPacket;
			pPacket = nullptr;
		}
	}

	return pPacket;
}
