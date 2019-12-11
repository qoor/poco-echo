#pragma once

class CMessageSendPacket : public CPacket
{
private:
	char* m_szMessage;

public:
	CMessageSendPacket(const char* szMessage);
	~CMessageSendPacket();

	bool Write(FIFOBuffer* pBuffer);

	ePacketType GetPacketType()
	{
		return PACKET_TYPE_MESSAGE;
	}
};
