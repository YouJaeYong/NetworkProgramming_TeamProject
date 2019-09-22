#include "stdafx.h"
#include "UI.h"
#include "BitmapMgr.h"
#include "TimeMgr.h"

CUI::CUI(int iX, int iY, IMAGE_INFO _tImageInfo, E_UI_LEVEL _UILevel, bool _Visible)
{
	m_tImageInfo = _tImageInfo;
	m_IsActionFrame = false;
	m_iX = iX;
	m_iY = iY;

	m_eUILevel = _UILevel;

	memset(m_strKey, 0, sizeof(m_strKey));

	m_IsVisible = _Visible;
	m_IsDestroy = false;

	m_pCallBack.PFUNC = NULL;
}


CUI::~CUI()
{
	if (m_pCallBack.PFUNC)
	{
		m_pCallBack.PFUNC();
	}
}

void CUI::Update(void)
{
	if (m_IsActionFrame == true)
	{
		m_tFrame.m_fFrameTime += GETTIME;
		if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
		{
			m_tFrame.m_fFrameTime = 0;
			m_tFrame.m_iCurFrame++;
		}

		if (m_tFrame.m_iCurFrame >= m_tFrame.m_iMaxFrame)
		{
			m_tFrame.m_iCurFrame = m_tFrame.m_iFirstFrame;
		}
	}

}

void CUI::Render(void)
{
	if (m_IsVisible == false)return;

	TransparentBlt(
		CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"),
		int(m_iX - m_tImageInfo.iImage_Width / 2),
		int(m_iY - m_tImageInfo.iImage_Height / 2),
		m_tImageInfo.iImage_Width,
		m_tImageInfo.iImage_Height,
		CBitmapMgr::GetInstance()->GetMemDC(m_tImageInfo.strImageName),
		m_tFrame.m_iCurFrame * m_tImageInfo.iImage_Width,
		m_tFrame.m_iScene * m_tImageInfo.iImage_Height,
		m_tImageInfo.iImage_Width,
		m_tImageInfo.iImage_Height,
		m_tImageInfo.m_iRGB
	);

}

void CUI::ChangeImage(const TCHAR * ImageName)
{
	lstrcpy(m_tImageInfo.strImageName, ImageName);
}
