#pragma once

class CPacketHandler : public CSingleton<CPacketHandler>
{
public:
	CPacketHandler() { }

	bool ProcessPacket(FIFOBuffer* pBuffer, ePacketType packetType);
};
