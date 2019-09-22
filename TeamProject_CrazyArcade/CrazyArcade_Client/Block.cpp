#include "stdafx.h"
#include "Block.h"
#include "ObjectMgr.h"
#include "TimeMgr.h"
#include "MapMgr.h"
#include "BitmapMgr.h"
//#include "Item.h"
#include "ObjectMgr.h"
#include"Item.h"
CBlock::CBlock(int iID, float fX, float fY, IMAGE_INFO _ImageInfo, E_BLOCK_TYPE eBlockType, int iItemType)
	: CObject(iID, _ImageInfo)
{
	m_tInfo.vPos.x = fX;
	m_tInfo.vPos.y = fY;

	m_eBlockType = eBlockType;
	m_dwBlockType = 0;

	m_eRenderType = E_RENDER_TYPE_SORT_OBJECT;
	CObjectMgr::GetInstance()->AddRenderer(this);
	m_pBushIsin = NULL;

	//if (iItemType >= E_ITEMTYPE_KICK)
	//{
	//	iItemType = rand() % 6;
	//}
	m_eItemType = (E_ITEMTYPE)iItemType;
}

CBlock::~CBlock()
{
	if (m_eBlockType == E_BLOCK_TYPE_BUSH)
	{
		CMapMgr::GetInstance()->DeleteBush(this);
	}

	if ((m_eBlockType == E_BLOCK_TYPE_BASIC || m_eBlockType == E_BLOCK_TYPE_PUSH)
		&& m_eItemType != E_ITEMTYPE_END)
	{
		CalcTileCenter(m_tInfo.vPos, &m_tInfo.vPos);

		//아이템 생성
		CItem* pItem = new CItem(m_tInfo.vPos, IMAGE_INFO(L"Item", 40, 40, RGB(0, 255, 0)), m_eItemType);
		CObjectMgr::GetInstance()->AddObject(pItem);

	}
}

void CBlock::Initialize(void)
{
	switch (m_eBlockType)
	{
	case E_BLOCK_TYPE_BASIC:
		m_dwBlockType = BLOCK_TYPE_BASIC;
		m_tFrame = FRAME(0, 0, 3, 0, 0.07f);
		break;
	case E_BLOCK_TYPE_PUSH:
		m_dwBlockType = BLOCK_TYPE_PUSH;
		m_tFrame = FRAME(0, 0, 3, 0, 0.07f);
		break;
	case E_BLOCK_TYPE_BUSH:
		m_dwBlockType = BLOCK_TYPE_BUSH;
		m_tFrame = FRAME(0, 0, 5, 0, 0.15f);
		break;
	case E_BLOCK_TYPE_STATIC:
		m_dwBlockType = BLOCK_TYPE_STATIC;
		break;
	}


	int iOffsetX = 20;
	int iOffsetY = 40;

	int iX = 0;
	int iY = 0;

	if (CalcTileIndex(m_tInfo.vPos, &iX, &iY) == false) return;

	m_rcRectSize.left = -TILECX;
	m_rcRectSize.right = TILECX;

	m_rcRectSize.top = -TILECY;
	m_rcRectSize.bottom = TILECY;


	if (m_eBlockType != E_BLOCK_TYPE_BUSH)
	{
		m_tInfo.rcRect.left = long(iX * TILECX + iOffsetX) + 2;
		m_tInfo.rcRect.right = long(iX * TILECX + TILECX + iOffsetX) - 2;

		m_tInfo.rcRect.top = long(iY * TILECY + iOffsetY) + 2;
		m_tInfo.rcRect.bottom = long(iY * TILECX + TILECY + iOffsetY) - 2;
	}
	else
	{
		m_tInfo.rcRect.left = long(iX * TILECX + iOffsetX) + 10;
		m_tInfo.rcRect.right = long(iX * TILECX + TILECX + iOffsetX) - 10;

		m_tInfo.rcRect.top = long(iY * TILECY + iOffsetY) + 10;
		m_tInfo.rcRect.bottom = long(iY * TILECX + TILECY + iOffsetY) - 10;
	}


	m_vRectCenter.x = float((m_tInfo.rcRect.left + m_tInfo.rcRect.right) / 2);
	m_vRectCenter.y = float((m_tInfo.rcRect.top + m_tInfo.rcRect.bottom) / 2);
}

void CBlock::Update(void)
{
	if (m_IsPlayAni == true)
	{
		PlayAnimation();
	}

	if (m_pBushIsin)
	{
		RECT rc;

		if (IntersectRect(&rc, &m_pBushIsin->GetInfo()->rcRect, &m_tInfo.rcRect) == false)
		{
			m_pBushIsin = NULL;
		}
	}


}
void CBlock::Render(void)
{
	CObject::Render();

	/*if (m_IsVisible == false || m_IsDestory)return;

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

	);*/
}

void CBlock::SetBushIsIn(CObject * pObj)
{
	if (pObj == NULL || pObj->IsDestory()) return;

	if (m_eBlockType == E_BLOCK_TYPE_BUSH)
	{
		if (m_pBushIsin != pObj)
		{
			m_pBushIsin = pObj;
			m_IsPlayAni = true;
		}
	}

}

void CBlock::CalcRect(void)
{
	int iOffsetX = 20;
	int iOffsetY = 40;

	int iX = 0;
	int iY = 0;

	if (CalcTileIndex(m_tInfo.vPos, &iX, &iY) == false) return;

	m_rcRectSize.left = -TILECX;
	m_rcRectSize.right = TILECX;

	m_rcRectSize.top = -TILECY;
	m_rcRectSize.bottom = TILECY;


	if (m_eBlockType != E_BLOCK_TYPE_BUSH)
	{
		m_tInfo.rcRect.left = long(iX * TILECX + iOffsetX) + 2;
		m_tInfo.rcRect.right = long(iX * TILECX + TILECX + iOffsetX) - 2;

		m_tInfo.rcRect.top = long(iY * TILECY + iOffsetY) + 2;
		m_tInfo.rcRect.bottom = long(iY * TILECX + TILECY + iOffsetY) - 2;
	}
	else
	{
		m_tInfo.rcRect.left = long(iX * TILECX + iOffsetX) + 5;
		m_tInfo.rcRect.right = long(iX * TILECX + TILECX + iOffsetX) - 5;

		m_tInfo.rcRect.top = long(iY * TILECY + iOffsetY) + 5;
		m_tInfo.rcRect.bottom = long(iY * TILECX + TILECY + iOffsetY) - 5;
	}


	m_vRectCenter.x = float((m_tInfo.rcRect.left + m_tInfo.rcRect.right) / 2);
	m_vRectCenter.y = float((m_tInfo.rcRect.top + m_tInfo.rcRect.bottom) / 2);
}

void CBlock::PlayAnimation(void)
{
	m_tFrame.m_fFrameTime += GETTIME;
	switch (m_eBlockType)
	{
	case E_BLOCK_TYPE_PUSH:
	case E_BLOCK_TYPE_BASIC:
	{
		if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
		{
			m_tFrame.m_fFrameTime = 0;
			m_tFrame.m_iCurFrame++;
			if (m_tFrame.m_iCurFrame >= m_tFrame.m_iMaxFrame)
			{
				if (m_tFrame.m_iScene >= 2)
				{
					m_tFrame.m_iCurFrame = 0;
					m_IsPlayAni = false;//스탑
					m_IsDestory = true;//삭제
				}
				else
				{
					m_tFrame.m_iCurFrame = 0;
					m_tFrame.m_iScene++;
				}

			}
		}

	}
	break;
	case E_BLOCK_TYPE_BUSH:
	{
		if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
		{
			m_tFrame.m_fFrameTime = 0;
			m_tFrame.m_iCurFrame++;
			if (m_tFrame.m_iCurFrame >= m_tFrame.m_iMaxFrame)
			{
				m_tFrame.m_iCurFrame = 0;
				m_IsPlayAni = false;//스탑
			}
		}
	}
	break;
	case E_BLOCK_TYPE_STATIC:
		break;
	default:
		m_tFrame.m_fFrameTime = 0;
		break;
	}
}
