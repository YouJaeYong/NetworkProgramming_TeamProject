#pragma once
#include "Item.h"
class CDart :
	public CItem
{
public:
	CDart(VECTOR2 vPos, VECTOR2  vDir, int iItemType);
	virtual ~CDart();
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	virtual void CalcRect();
private:
	float			m_fSpeed;
};

