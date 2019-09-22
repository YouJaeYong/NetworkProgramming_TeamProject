#include "stdafx.h"
#include "Item.h"
#include "Character.h"

CItem::CItem(VECTOR2 vPos, IMAGE_INFO _tImageInfo, int iItemType)
	: CObject(0, _tImageInfo)
{
	m_eItemType = (E_ITEMTYPE)iItemType;
	if (m_eItemType < E_ITEMTYPE_BANANA)
		m_eRenderType = E_RENDER_TYPE_SORT_OBJECT;
	else
		m_eRenderType = E_RENDER_TYPE_ITEM;


	m_eObjectListType = E_OBJ_LIST_TYPE_ITEM;
	m_tInfo.vPos = vPos;
	m_tInfo.vPos.y -= 5.f;
	m_IsMoveUp = true;
	m_fDist = 0.f;

	m_pOwner = NULL;
	Initialize();
}
CItem::~CItem()
{
	m_pOwner = NULL;
}


void CItem::Initialize(void)
{
	m_tInfo.rcRect.left = long(m_tInfo.vPos.x - TILECX / 4);
	m_tInfo.rcRect.right = long(m_tInfo.vPos.x + TILECX / 4);
	m_tInfo.rcRect.top = long(m_tInfo.vPos.y - TILECY / 4);
	m_tInfo.rcRect.bottom = long(m_tInfo.vPos.y + TILECY / 4);


	m_tFrame = FRAME(m_eItemType, 0, 0, 0, 0);
}

void CItem::Update(void)
{
	if (m_IsDestory)return;


	if (m_eItemType <= E_ITEMTYPE_KICK)//압정과 바나나 제외
	{
		if (m_fDist > 2.f)
		{
			m_fDist = 0.f;
			m_IsMoveUp = !m_IsMoveUp;
		}

		if (m_IsMoveUp)
		{
			m_fDist += 0.1f;
			m_tInfo.vPos.y += 0.1f;
		}
		else
		{
			m_fDist += 0.1f;
			m_tInfo.vPos.y -= 0.1f;
		}
	}

	if (m_pOwner)//바나나 처음에는 충돌 처리 않하기 위해서
	{
		if (m_pOwner->GetState() == E_CHARACTER_STATE_DEATH)
		{
			m_pOwner = NULL;
		}
		else
		{
			RECT rc;
			if (IntersectRect(&rc, &m_pOwner->GetInfo()->rcRect, &m_tInfo.rcRect) == false)
			{
				m_pOwner = NULL;//이때 부터 슬라이딩 가능
			}
		}
	}


}

void CItem::Render(void)
{
	if (m_IsDestory)return;
	CObject::Render();
}
