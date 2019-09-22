#pragma once
#include "UI.h"
class CUI_ImageFont :
	public CUI
{
public:
	CUI_ImageFont(const int* psec, int iX, int iY, IMAGE_INFO _tImageInfo, E_UI_LEVEL _UILevel, bool _Visible = true);
	virtual ~CUI_ImageFont();

public:
	virtual void Update(void);
	virtual void Render(void);
private:
	const int*					m_pNum;
};
