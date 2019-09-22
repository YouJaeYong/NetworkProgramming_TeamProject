#include "stdafx.h"
#include "Tile.h"
#include "BitmapMgr.h"

CTile::CTile(int iX, int iY)
{
	m_iX = iX;
	m_iY = iY;
}


CTile::~CTile()
{
}

void CTile::SetImageName(TCHAR * strName)
{
	lstrcpy(m_tImageInfo.strImageName, strName);
}

void CTile::SetFrame(int _iFrame)
{
	m_tFrame.m_iCurFrame = _iFrame;
	//	m_tFrame.m_iCurFrame = min(int(m_tFrame.m_fMaxFrameTime), m_tFrame.m_iCurFrame);
	//	m_tFrame.m_iCurFrame = max(0, m_tFrame.m_iCurFrame);
}

void CTile::Render(void)
{
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
