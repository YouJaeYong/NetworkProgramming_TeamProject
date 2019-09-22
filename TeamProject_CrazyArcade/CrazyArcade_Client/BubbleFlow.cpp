#include "stdafx.h"
#include "BubbleFlow.h"
#include "TimeMgr.h"
#include "BitmapMgr.h"
#include "GameMgr.h"

CBubbleFlow::CBubbleFlow(VECTOR2 vPos, IMAGE_INFO _tImageInfo, FRAME _tFrame, const char* szKey)
	: CObject(0, _tImageInfo)
{
	m_tInfo.vPos = vPos;
	m_tFrame = _tFrame;

	strcpy_s(m_szKey, sizeof(m_szKey), szKey);

	Initialize();
}



CBubbleFlow::~CBubbleFlow()
{
}

void CBubbleFlow::Initialize(void)
{
	m_tInfo.rcRect.left = long(m_tInfo.vPos.x - TILECX / 3);
	m_tInfo.rcRect.right = long(m_tInfo.vPos.x + TILECX / 3);

	m_tInfo.rcRect.top = long(m_tInfo.vPos.y - TILECY / 3);
	m_tInfo.rcRect.bottom = long(m_tInfo.vPos.y + TILECY / 3);

}

void CBubbleFlow::Update(void)
{
	if (m_IsDestory == true)return;

	m_tFrame.m_fFrameTime += GETTIME;
	if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
	{
		m_tFrame.m_fFrameTime = 0;
		m_tFrame.m_iCurFrame++;

		if (m_tFrame.m_iMaxFrame < m_tFrame.m_iCurFrame)
		{
			m_tFrame.m_iCurFrame = 0;
			m_IsDestory = true;
		}
	}
}

void CBubbleFlow::Render(void)
{
	if (m_IsVisible == false || m_IsDestory)return;

	TransparentBlt(
		CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"),
		int(m_tInfo.vPos.x - m_tImageInfo.iImage_Width / 2),
		int(m_tInfo.vPos.y - m_tImageInfo.iImage_Height / 2),
		m_tImageInfo.iImage_Width,
		m_tImageInfo.iImage_Height,
		CBitmapMgr::GetInstance()->GetMemDC(m_tImageInfo.strImageName),
		m_tFrame.m_iCurFrame * m_tImageInfo.iImage_Width,
		m_tFrame.m_iScene * m_tImageInfo.iImage_Height,
		m_tImageInfo.iImage_Width,
		m_tImageInfo.iImage_Height,
		m_tImageInfo.m_iRGB
	);

	if (CGameMgr::GetInstance()->IsCollBoxRender())
		Rectangle(m_hDC, m_tInfo.rcRect.left, m_tInfo.rcRect.top, m_tInfo.rcRect.right, m_tInfo.rcRect.bottom);
}
