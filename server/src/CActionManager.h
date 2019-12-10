#pragma once

class CActionManager : public CSingleton<CActionManager>
{
private:
	CWhiteList* m_pWhiteList;

public:
	CActionManager(CWhiteList* pWhiteList);

	void ShowHelp();
	bool PopActionResult(int* piDestActionType, void** pDestination, int* piDestSize);

private:
	bool GetAction(int iAction);
};
