#pragma once
#include "Object.h"
class CBubbleFlow :
	public CObject
{
public:
	CBubbleFlow(VECTOR2 vPos, IMAGE_INFO _tImageInfo, FRAME _tFrame, const char* szKey);
	virtual ~CBubbleFlow();

public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	const char* GetKey(void) { return m_szKey; }
private:
	char			m_szKey[30];
};

