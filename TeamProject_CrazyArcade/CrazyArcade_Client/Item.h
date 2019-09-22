#pragma once
#include "Object.h"

class CCharacter;
class CItem :
	public CObject
{
public:
	CItem(VECTOR2 vPos, IMAGE_INFO _tImageInfo, int iItemType);
	virtual ~CItem();


public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	E_ITEMTYPE	GetItemType(void) { return m_eItemType; }

	void SetOwner(CCharacter* pCharacter) { m_pOwner = pCharacter; }
	CCharacter* GetOwner(void) { return m_pOwner; }
private:
	bool					m_IsMoveUp;
	float					m_fDist;
	CCharacter*				m_pOwner;
protected:
	E_ITEMTYPE				m_eItemType;

};

