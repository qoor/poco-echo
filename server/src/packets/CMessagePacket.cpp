#include "StdInc.h"

CMessagePacket::CMessagePacket()
{
	m_szMessage = nullptr;
}

CMessagePacket::~CMessagePacket()
{
	if (m_szMessage)
	{
		delete[] m_szMessage;
		m_szMessage = nullptr;
	}
}

bool CMessagePacket::Read(FIFOBuffer* pBuffer)
{
	try
	{
		int iLength;
		StreamSocket* pClient = GetSocket();

		pBuffer->read(reinterpret_cast<char*>(&iLength), sizeof(int));

		if (m_szMessage)
		{
			delete[] m_szMessage;
		}

		m_szMessage = new char[iLength];
		assert(m_szMessage);

		pBuffer->read(m_szMessage, iLength);
	}
	catch (Exception)
	{
		return false;
	}

	return true;
}
