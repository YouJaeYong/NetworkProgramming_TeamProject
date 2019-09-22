#pragma once
#include "UI.h"
class CUI_Font :
	public CUI
{
public:
	CUI_Font(int iX, int iY, const char* Text, E_UI_LEVEL _UILevel, UINT iColor = RGB(0, 0, 0), const TCHAR* Face = L"Arial", bool IsVisible = true);
	virtual ~CUI_Font();

	virtual void Update(void) {}
	virtual void Render(void);
private:
	TCHAR			m_strText[128];
	UINT			m_FontColor;

	HFONT			m_newFont;
	HFONT			m_oldFont;

};
