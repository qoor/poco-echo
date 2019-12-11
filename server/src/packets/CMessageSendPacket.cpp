#include "StdInc.h"

CMessageSendPacket::CMessageSendPacket(const char* szMessage)
{
	assert(szMessage);

	size_t messageLength = strlen(szMessage) + 1;

	m_szMessage = new char[messageLength];
	strncpy(m_szMessage, szMessage, messageLength);
}

CMessageSendPacket::~CMessageSendPacket()
{
	if (m_szMessage)
	{
		delete m_szMessage;
		m_szMessage = nullptr;
	}
}

bool CMessageSendPacket::Write(FIFOBuffer* pBuffer)
{
	if (!m_szMessage)
	{
		return false;
	}

	ePacketType packetType = PACKET_TYPE_MESSAGE;
	int messageLength = strlen(m_szMessage) + 1;

	pBuffer->write(reinterpret_cast<char*>(&packetType), sizeof(ePacketType));
	pBuffer->write(reinterpret_cast<char*>(&messageLength), sizeof(int));
	pBuffer->write(m_szMessage, messageLength);
	return true;
}
