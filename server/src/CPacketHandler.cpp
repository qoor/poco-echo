#include "StdInc.h"

CPacketHandler* CSingleton<CPacketHandler>::m_pInstance = nullptr;

void CPacketHandler::ProcessPacket(StreamSocket* pClient, Poco::FIFOBuffer* pBuffer, ePacketType packetType)
{
	assert(pClient);
	assert(pBuffer);

	switch (packetType)
	{
		case PACKET_TYPE_MESSAGE:
		{
			int iLength;
			char* szMessage;
			char* szSendMessage;
			int iSendLength;

			pBuffer->read(reinterpret_cast<char*>(&iLength), sizeof(int));

			szMessage = new char[iLength + 1];
			assert(szMessage);

			pBuffer->read(szMessage, iLength);
			szMessage[iLength] = '\0';

			std::cout << "Ŭ���̾�Ʈ (" << pClient->address().toString() << ")(��)�� ���� �޼��� ����: " << szMessage << "\n";

			szSendMessage = new char[iLength + 1 + 32];
			assert(szSendMessage);

			strncpy(szSendMessage, szMessage, iLength);
			szSendMessage[iLength] = '\0';

			strcat(szSendMessage, " (Server: Ok!)");

			iSendLength = strlen(szSendMessage);
			pClient->sendBytes(reinterpret_cast<char*>(&iSendLength), sizeof(int));
			pClient->sendBytes(szSendMessage, iSendLength);

			delete[] szMessage;
			delete[] szSendMessage;

			break;
		}

		default:
		{
			std::cout << "Ŭ���̾�Ʈ (" << pClient->address().toString() << ")(��)�� ���� �� �� ���� ��Ŷ�� ���Խ��ϴ�. (id:" << packetType << "\n";
		}
	}
}

