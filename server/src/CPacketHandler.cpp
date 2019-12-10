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

			std::cout << "클라이언트 (" << pClient->address().toString() << ")(으)로 부터 메세지 받음: " << szMessage << "\n";

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
			std::cout << "클라이언트 (" << pClient->address().toString() << ")(으)로 부터 알 수 없는 패킷이 들어왔습니다. (id:" << packetType << "\n";
		}
	}
}

