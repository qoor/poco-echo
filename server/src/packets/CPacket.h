#pragma once

class CPacket
{
private:
	StreamSocket* m_pSocket;

public:
	CPacket();
	virtual ~CPacket() { }

	virtual ePacketType GetPacketType() = 0;

	virtual bool Read(FIFOBuffer* pBuffer) { return false; }
	virtual bool Write(FIFOBuffer* pBuffer) { return false; }

	void SetSocket(StreamSocket* pSocket)
	{
		m_pSocket = pSocket;
	}
	StreamSocket* GetSocket()
	{
		return m_pSocket;
	}
};
