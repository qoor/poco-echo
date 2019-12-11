#pragma once

class CActionManager : public Poco::Runnable, CSingleton<CActionManager>
{
private:
	CWhiteList* m_pWhiteList;

public:
	CActionManager(CWhiteList* pWhiteList);

	void ShowHelp();
	bool PopActionResult(int* piDestActionType, void** pDestination, int* piDestSize);

	virtual void run();

private:
	bool GetAction(int iAction);
};
