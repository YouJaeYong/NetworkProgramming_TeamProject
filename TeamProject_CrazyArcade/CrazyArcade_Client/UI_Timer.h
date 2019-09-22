#pragma once
#include "UI.h"
class CUI_Timer :
	public CUI
{
public:
	CUI_Timer(int iX, int iY, float fStartTime, float fLifeTime, IMAGE_INFO _tImageInfo, E_UI_LEVEL _UILevel, bool _Visible = true);
	virtual ~CUI_Timer();

public:
	virtual void Update(void);
	virtual void Render(void);
private:
	float				m_fStartTime;
	float				m_fCurStartTime;

	float				m_fCurLifeTime;
	float				m_fLifeTime;
};

