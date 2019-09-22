#pragma once
#include "Object.h"


class CBlock :
	public CObject
{
public:
	CBlock(int iID, float fX, float fY, IMAGE_INFO _ImageInfo, E_BLOCK_TYPE eBlockType, int iItemType = E_ITEMTYPE_END);
	virtual ~CBlock();
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	E_BLOCK_TYPE GetBlockType() { return m_eBlockType; }

	void SetBushIsIn(CObject* pObj);
	virtual void CalcRect(void);

private:
	virtual void PlayAnimation(void);
private:
	E_BLOCK_TYPE			m_eBlockType;
	DWORD					m_dwBlockType;

	CObject*				m_pBushIsin;//부쉬 안에 있음
	E_ITEMTYPE				m_eItemType;
};

