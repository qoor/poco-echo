#pragma once

template<typename T>
class CSingleton
{
private:
	static T* m_pInstance;

protected:
	CSingleton()
	{
		assert(m_pInstance != nullptr);
		m_pInstance = this;
	}

	static T* GetInstance()
	{
		return m_pInstance;
	}
};
