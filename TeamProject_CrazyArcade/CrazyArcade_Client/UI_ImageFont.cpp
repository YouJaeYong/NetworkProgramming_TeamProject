#include "stdafx.h"
#include "UI_ImageFont.h"

CUI_ImageFont::CUI_ImageFont(const int* pNum, int iX, int iY, IMAGE_INFO _tImageInfo, E_UI_LEVEL _UILevel, bool _Visible)
	: CUI(iX, iY, _tImageInfo, _UILevel, _Visible)
	, m_pNum(pNum)
{
}

CUI_ImageFont::~CUI_ImageFont()
{
}

void CUI_ImageFont::Update(void)
{
	if (m_pNum &&  *m_pNum > 0 && *m_pNum < 10)
	{
		m_tFrame.m_iCurFrame = *m_pNum;
	}
	else
	{
		m_tFrame.m_iCurFrame = 0;
	}
}

void CUI_ImageFont::Render(void)
{
	CUI::Render();
}