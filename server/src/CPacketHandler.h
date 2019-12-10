#pragma once

class CPacketHandler : public CSingleton<CPacketHandler>
{
public:
	CPacketHandler() { }

	void ProcessPacket(StreamSocket* pClient, Poco::FIFOBuffer* pBuffer, ePacketType packetType);
};
