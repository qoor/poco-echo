#pragma once

class CMessage : public CSingleton<CMessage>
{
public:
	static bool ShowMessage(FIFOBuffer* pBuffer);
};
