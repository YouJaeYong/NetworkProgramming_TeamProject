#include "stdafx.h"
#include "Object.h"
#include "BitmapMgr.h"
#include "GameMgr.h"
#include "ObjectMgr.h"
CObject::CObject(int _iID, IMAGE_INFO _tImageInfo)
{
	m_tImageInfo = _tImageInfo;

	m_IsVisible = true;
	m_IsDestory = false;

	m_IsPlayAni = false;

	m_tInfo.iID = _iID;
	m_hDC = GetDC(g_hWnd);
	m_eRenderType = E_RENDER_TYPE_END;
	m_eObjectListType = E_OBJ_LIST_END;

	memset(&m_rcRectSize, 0, sizeof(m_rcRectSize));

}


CObject::~CObject()
{
	Release();
}

void CObject::Initialize(void)
{
}

void CObject::Update(void)
{
}

void CObject::Render(void)
{
	if (m_IsVisible == false || m_IsDestory)
		return;
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

void CObject::Release(void)
{
	CObjectMgr::GetInstance()->DeleteRender(this);
}

void CObject::CalcRect(void)
{
	m_tInfo.rcRect.left = (long)(m_tInfo.vPos.x + m_rcRectSize.left);
	m_tInfo.rcRect.right = (long)(m_tInfo.vPos.x + m_rcRectSize.right);

	m_tInfo.rcRect.top = (long)(m_tInfo.vPos.y + m_rcRectSize.top);
	m_tInfo.rcRect.bottom = (long)(m_tInfo.vPos.y + m_rcRectSize.bottom);
}

bool CObject::CalcTileIndex(VECTOR2 vPos, int* pX, int* pY)
{
	int iOffsetX = 20;
	int iOffsetY = 40;

	*pX = int(vPos.x - iOffsetX) / TILECX;
	*pY = int(vPos.y - iOffsetY) / TILECY;

	if (*pX < 0 || *pY < 0 || *pX >= TILE_CNTX || *pY >= TILE_CNTY)return false;

	return true;
}

bool CObject::CalcTileCenter(VECTOR2 vPos, VECTOR2 * vCenter)
{
	int iOffsetX = 20;
	int iOffsetY = 40;

	int iX = 0;
	int iY = 0;
	if (CalcTileIndex(vPos, &iX, &iY) == false) return false;


	vCenter->x = (TILECX * iX + iOffsetX) + TILECX / 2.0f;
	vCenter->y = (TILECY * iY + iOffsetY) + TILECY / 2.0f;

	return true;
}
