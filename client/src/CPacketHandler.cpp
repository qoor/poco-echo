#include "StdInc.h"

#include "packet/CMessage.h"

CPacketHandler* CSingleton<CPacketHandler>::m_pInstance = nullptr;

// �Լ��� ��ȯ���� false�� ���� ������ ����� �Ѵٴ� ��
bool CPacketHandler::ProcessPacket(FIFOBuffer* pBuffer, ePacketType packetType)
{
	switch (packetType)
	{
		case PACKET_TYPE_KICK:
		{
			std::cout << "�������� ������ �ź��Ͽ����ϴ�." << "\n";
			return false;

		}

		case PACKET_TYPE_MESSAGE:
		{
			CMessage::ShowMessage(pBuffer);
			break;
		}

		default:
		{
			std::cout << "�� �� ���� ��Ŷ �������Դϴ�. (id: " << packetType << "\n";
		}
	}

	return true;
}
