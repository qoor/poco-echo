#pragma once

class CActionManager : public Poco::Runnable, CSingleton<CActionManager>
{
private:
	CClient* m_pClient;
	int m_iActionType;
	void* m_pActionResult;
	size_t m_actionResultSize;

public:
	CActionManager(CClient* pClient);

	void ShowHelp();
	bool PopActionResult(int* piDestActionType, void** pDestination, int* piDestSize);

	virtual void run();

private:
	bool GetAction(int iAction);
};
