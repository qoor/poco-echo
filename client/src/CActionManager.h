#pragma once

class CActionManager : public CSingleton<CActionManager>
{
private:
	CClient* m_pClient;

public:
	CActionManager(CClient* pClient);

	void ShowHelp();
	bool GetAction(int iAction);
};
