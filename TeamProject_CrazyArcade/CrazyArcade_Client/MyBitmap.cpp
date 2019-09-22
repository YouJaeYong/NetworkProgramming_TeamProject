#include "stdafx.h"
#include "MyBitmap.h"
#include "Include.h"


CMyBitmap::CMyBitmap()
{
}


CMyBitmap::~CMyBitmap()
{
	SelectObject(m_MemDC, m_OldBitmap);
	DeleteObject(m_Bitmap);
	DeleteDC(m_MemDC);

}

CMyBitmap * CMyBitmap::LoadBmp(TCHAR * pFileName)
{
	m_DC = GetDC(g_hWnd);
	//인자로 들어가는 DC와 호환되는 DC를 만든다..
	m_MemDC = CreateCompatibleDC(m_DC);

	m_Bitmap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (m_Bitmap == 0)
		MessageBox(g_hWnd, L"Image Load Failed", L"Error!!", MB_OK);
	m_OldBitmap = (HBITMAP)SelectObject(m_MemDC, m_Bitmap);

	ReleaseDC(g_hWnd, m_DC);

	return this;
}
