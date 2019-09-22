#pragma once

#define DECLARE_SINGLETON(ClassName)			\
private:										\
	static ClassName*		m_pInstance;		\
public:											\
	static ClassName*		GetInstance(void)	\
	{											\
		if(m_pInstance == NULL)					\
			m_pInstance = new ClassName;		\
		return m_pInstance;						\
	}											\
	void	DestroyInstance(void)				\
	{											\
		if(m_pInstance != NULL)					\
		{										\
			delete m_pInstance;					\
			m_pInstance = NULL;					\
		}										\
	}											\


#define  IMPLEMENT_SINGLETON(ClassName)			\
ClassName*	ClassName::m_pInstance = NULL;		\



#define SAFE_FUNC(p  , func)		if(p)p->func;