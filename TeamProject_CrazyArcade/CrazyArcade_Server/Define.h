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

#define						MIN_STR			40
const int						TILE_CNTX = 15;
const int						TILE_CNTY = 13;

const int				TILECX = 40;
const int				TILECY = 40;

