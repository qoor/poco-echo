#include "StdInc.h"

#include "packet/CMessage.h"

CPacketHandler* CSingleton<CPacketHandler>::m_pInstance = nullptr;

// 함수의 반환값이 false인 것은 연결을 끊어야 한다는 것
bool CPacketHandler::ProcessPacket(FIFOBuffer* pBuffer, ePacketType packetType)
{
	switch (packetType)
	{
		case PACKET_TYPE_KICK:
		{
			std::cout << "서버에서 연결을 거부하였습니다." << "\n";
			return false;

		}

		case PACKET_TYPE_MESSAGE:
		{
			CMessage::ShowMessage(pBuffer);
			break;
		}

		default:
		{
			std::cout << "알 수 없는 패킷 데이터입니다. (id: " << packetType << "\n";
		}
	}

	return true;
}
