#pragma once

class CMessagePacket : public CPacket
{
public:
	char* m_szMessage;

public:
	CMessagePacket();
	~CMessagePacket();

	ePacketType GetPacketType()
	{
		return PACKET_TYPE_MESSAGE;
	}

	bool Read(FIFOBuffer* pBuffer);
};
