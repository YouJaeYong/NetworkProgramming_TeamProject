#include "stdafx.h"
#include "UI_Font.h"
#include "BitmapMgr.h"

CUI_Font::CUI_Font(int iX, int iY, const char * Text, E_UI_LEVEL _UILevel, UINT iColor, const TCHAR * Face, bool IsVisible)
	: CUI(iX, iY, IMAGE_INFO(), _UILevel, IsVisible)
{
	MultiByteToWideChar(CP_ACP, 0, Text, 128, m_strText, 128);

	m_FontColor = iColor;

	m_newFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		VARIABLE_PITCH | FF_SWISS, Face);

	//	SetTextSize
	//GetConsoleFontSize()
	m_oldFont = (HFONT)SelectObject(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"), m_newFont);

}

CUI_Font::~CUI_Font()
{
	SelectObject(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"), m_oldFont);
	DeleteObject(m_newFont);
}

void CUI_Font::Render(void)
{
	if (m_IsVisible == false)return;

	HDC hDC = CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer");

	SetTextColor(hDC, m_FontColor);
	//SetTextCharacterExtra(hDC, 20);//간격 조절
	//SetTextAlign(hDC, TA_LEFT);//정렬
	TextOut(hDC, m_iX, m_iY, m_strText, lstrlen(m_strText));
	//	DrawText(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer") , m_strText , lstrlen(m_strText)  , 
	SetBkMode(hDC, TRANSPARENT);
}
