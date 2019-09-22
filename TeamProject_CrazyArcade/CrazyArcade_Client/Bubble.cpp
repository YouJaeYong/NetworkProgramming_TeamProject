#include "stdafx.h"
#include "Bubble.h"
#include "Character.h"
#include "TimeMgr.h"
#include "ObjectMgr.h"
#include "Block.h"
#include "MapMgr.h"
#include "CollisionMgr.h"
#include "BubbleFlow_List.h"
#include"SoundMgr.h"
CBubble::CBubble(int iID, VECTOR2 vPos, IMAGE_INFO _tImage, CCharacter* pOwner, int iPower, float fLifeTime)
	: CObject(iID, _tImage)
{
	m_eObjectListType = E_OBJ_LIST_TYPE_BUBBLE;
	m_eRenderType = E_RENDER_TYPE_SORT_OBJECT;

	m_tInfo.vPos = vPos;

	m_pOwner = pOwner;
	m_fLifeTime = fLifeTime;

	if (m_pOwner->IsDestory() == false)
		++m_pOwner->GetCharacterInfo()->m_tItemInfo.iCurBubble;//사용
	else
		m_pOwner = NULL;

	m_fCurTime = 0.f;
	m_iPower = iPower;
	m_tFrame = FRAME(0, 0, 4, 0, 0.25f);
	m_fKickSpeed = 5.f;
	m_IsKick = false;
	m_IsCreateFlow = false;
}


CBubble::~CBubble()
{
	Release();
}

void CBubble::Initialize(void)
{

	CSoundMgr::GetInstance()->PlayPlayer(L"SFX_Bubble_On.wav", g_fVolume);

	m_tInfo.rcRect.left = long(m_tInfo.vPos.x - TILECX / 2);
	m_tInfo.rcRect.right = long(m_tInfo.vPos.x + TILECX / 2);
	m_tInfo.rcRect.top = long(m_tInfo.vPos.y - TILECY / 2);
	m_tInfo.rcRect.bottom = long(m_tInfo.vPos.y + TILECY / 2);

	m_vecObject = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_PLAYER);
}

void CBubble::Update(void)
{
	if (m_IsDestory)
		return;
	CCollisionMgr::GetInstance()->CollisionBubbleToItem(this);//아이템 충돌 체크

	if (m_pOwner && m_pOwner->GetState() == E_CHARACTER_STATE_DEATH)
	{
		--m_pOwner->GetCharacterInfo()->m_tItemInfo.iCurBubble;
		m_pOwner = NULL;
	}

	if (m_IsCreateFlow)
	{
		CSoundMgr::GetInstance()->PlayEffect(L"SFX_Bubble_Off.wav", g_fVolume);

		CBubbleFlow_List* pList = CBubbleFlow_List::Create(m_tInfo.vPos, m_iPower);
		CObjectMgr::GetInstance()->AddObject(pList);
		m_IsDestory = true;
		return;
	}

	m_fCurTime += GETTIME;
	if (m_fCurTime > m_fLifeTime)
	{
		CreateBubbleFlow();//서버에서 처리
		return;
	}

	m_tFrame.m_fFrameTime += GETTIME;
	if (m_tFrame.m_fFrameTime > m_tFrame.m_fMaxFrameTime)
	{
		m_tFrame.m_fFrameTime = 0;
		m_tFrame.m_iCurFrame++;
		if (m_tFrame.m_iCurFrame >= m_tFrame.m_iMaxFrame)
		{
			m_tFrame.m_iCurFrame = 0;
		}
	}
	vector<CObject*>::iterator iter = m_vecObject.begin();
	vector<CObject*>::iterator iter_end = m_vecObject.end();
	for (iter; iter != m_vecObject.end(); )
	{
		RECT rc;
		if (IntersectRect(&rc, &(*iter)->GetInfo()->rcRect, &m_tInfo.rcRect) == false)
		{
			iter = m_vecObject.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	KickMove();
}

void CBubble::Render(void)
{
	m_IsVisible = true;

	vector<CBlock*>	vecBlcok = *CMapMgr::GetInstance()->GetVecBush();

	for (size_t i = 0; i < vecBlcok.size(); ++i)
	{
		RECT rc;
		if (IntersectRect(&rc, &vecBlcok[i]->GetInfo()->rcRect, &m_tInfo.rcRect))
		{
			m_IsVisible = false;
		}
	}
	CObject::Render();
}

CBubble * CBubble::Create(int iID, VECTOR2 vPos, CCharacter * pOwner, int iPower, float fLifeTime /*= 3.f*/)
{
	IMAGE_INFO tImage = IMAGE_INFO(L"Bubble", 40, 40, RGB(0, 255, 0));

	CBubble* pBubble = new CBubble(iID, vPos, tImage, pOwner, iPower, fLifeTime);
	pBubble->Initialize();
	return pBubble;
}

void CBubble::CalcRect(void)
{
	m_tInfo.rcRect.left = long(m_tInfo.vPos.x - TILECX / 2);
	m_tInfo.rcRect.right = long(m_tInfo.vPos.x + TILECX / 2);
	m_tInfo.rcRect.top = long(m_tInfo.vPos.y - TILECY / 2);
	m_tInfo.rcRect.bottom = long(m_tInfo.vPos.y + TILECY / 2);
}

void CBubble::SetKick(bool _IsKick, VECTOR2 _vDir)
{
	if (_IsKick)
		CSoundMgr::GetInstance()->PlayPlayer(L"SFX_Character_KickBubble.wav", g_fVolume);

	m_IsKick = _IsKick;
	m_tInfo.m_vDir = _vDir;
	m_tInfo.m_vDir.Nomalize();

	m_vecObject = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_PLAYER);


}

void CBubble::CreateBubbleFlow()
{
	if (m_IsCreateFlow)return;

	m_IsCreateFlow = true;
}

void CBubble::BoomBubble()
{
	//	CreateBubbleFlow();
	CBubbleFlow_List* pList = CBubbleFlow_List::Create(m_tInfo.vPos, m_iPower);
	CObjectMgr::GetInstance()->AddObject(pList);

	if (m_IsCreateFlow)
		return;
	m_IsCreateFlow = true;
}

void CBubble::KickMove(void)
{
	if (m_IsKick == true)
	{
		VECTOR2 vDestPos;
		m_fKickSpeed = 200;
		VECTOR2 vMovePos = m_tInfo.vPos + m_tInfo.m_vDir * m_fKickSpeed * GETTIME;
		RECT rc;
		rc.left = 40;
		rc.top = 60;
		rc.right = 600;
		rc.bottom = 540;

		if (CCollisionMgr::GetInstance()->ScreenOutCheck(m_tInfo.vPos, vMovePos, &vDestPos, rc))
		{
			CalcTileCenter(vDestPos, &vDestPos);
			m_IsKick = false;
		}
		else if (CCollisionMgr::GetInstance()->CollisionObjectToBlock(this, vDestPos, &vDestPos))
		{
			CalcTileCenter(vDestPos, &vDestPos);
			m_IsKick = false;
		}
		else if (CCollisionMgr::GetInstance()->CollisionObjectToBubble(this, vDestPos, &vDestPos))
		{
			CalcTileCenter(vDestPos, &vDestPos);
			m_IsKick = false;
		}
		m_tInfo.vPos = vDestPos;
		CalcRect();
	}
}

void CBubble::Release(void)
{
	BoomBubble();// 플로우 생성
	if (m_pOwner)
		--m_pOwner->GetCharacterInfo()->m_tItemInfo.iCurBubble;
}
