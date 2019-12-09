#pragma once

class COption : public CSingleton<COption>
{
private:
	char* m_szServerAddress;
	int m_iServerPort;

public:
	COption();
	~COption();

	bool ParseOptions(int argc, char** argv);

	const char* GetServerAddress() const
	{
		return m_szServerAddress;
	}

	int GetServerPort()
	{
		return m_iServerPort;
	}
};
