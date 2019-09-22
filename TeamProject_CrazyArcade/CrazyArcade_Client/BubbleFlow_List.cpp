#include "stdafx.h"
#include "BubbleFlow_List.h"
#include "BubbleFlow.h"
#include "CollisionMgr.h"
#include "TimeMgr.h"
CBubbleFlow_List::CBubbleFlow_List(VECTOR2 vCenter, int iPower, E_FLOW_STATE eCenterFlow)
	: CObject(0, IMAGE_INFO())
{
	m_tInfo.vPos = vCenter;
	m_iPower = iPower;

	m_eObjectListType = E_OBJ_LIST_TYPE_BUBBLE_FLOW;
	m_eRenderType = E_RENDER_TYPE_BUBBLE_FLOW;
}


CBubbleFlow_List::~CBubbleFlow_List()
{
}

void CBubbleFlow_List::Initialize(void)
{
	IMAGE_INFO tImage = IMAGE_INFO(L"BubbleFlow", 40, 40, RGB(0, 255, 0));
	float fFrameTime = 0.13f;

	//센터
	CBubbleFlow* pBubbleFlow = new CBubbleFlow(m_tInfo.vPos, tImage, FRAME(0, E_FLOW_CENTER, 4, 0, fFrameTime), "Center");
	m_vecBubbleFlow.push_back(pBubbleFlow);

	for (int i = 1; i <= m_iPower; ++i)
	{
		//위
		pBubbleFlow = new CBubbleFlow(VECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y - i * TILECY), tImage, FRAME(0, E_FLOW_UP, 4, 0, fFrameTime), "Up");
		m_vecBubbleFlow.push_back(pBubbleFlow);
	}
	for (int i = 1; i <= m_iPower; ++i)
	{
		//아래
		pBubbleFlow = new CBubbleFlow(VECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y + i * TILECY), tImage, FRAME(0, E_FLOW_DOWN, 4, 0, fFrameTime), "Down");
		m_vecBubbleFlow.push_back(pBubbleFlow);
	}
	for (int i = 1; i <= m_iPower; ++i)
	{
		//오른쪽
		pBubbleFlow = new CBubbleFlow(VECTOR2(m_tInfo.vPos.x + i * TILECX, m_tInfo.vPos.y), tImage, FRAME(0, E_FLOW_RIGHT, 4, 0, fFrameTime), "Right");
		m_vecBubbleFlow.push_back(pBubbleFlow);
	}
	for (int i = 1; i <= m_iPower; ++i)
	{
		//왼쪽
		pBubbleFlow = new CBubbleFlow(VECTOR2(m_tInfo.vPos.x - i * TILECX, m_tInfo.vPos.y), tImage, FRAME(0, E_FLOW_LEFT, 4, 0, fFrameTime), "Left");
		m_vecBubbleFlow.push_back(pBubbleFlow);
	}

	////블록 충돌
	CCollisionMgr::GetInstance()->CheckFlowToBlock(this);
	//////물줄기 충돌체크
	////풍선 충돌체크
	CCollisionMgr::GetInstance()->CheckFlowToBubble(this);
	////밖으로 나가는거 체크
	CCollisionMgr::GetInstance()->CheckFlowToWindow(this);
	////아이템 충돌체크
	CCollisionMgr::GetInstance()->CheckFlowToItem(this);
	////플레이어 충돌 체크
	CCollisionMgr::GetInstance()->CheckFlowToPlayer(this);
}

void CBubbleFlow_List::Update(void)
{
	if (m_IsDestory)return;

	if (m_vecBubbleFlow.empty())
	{
		m_IsDestory = true;
		return;
	}

	vector<CBubbleFlow*>::iterator iter = m_vecBubbleFlow.begin();
	vector<CBubbleFlow*>::iterator iter_end = m_vecBubbleFlow.end();

	for (iter; iter != m_vecBubbleFlow.end(); )
	{
		(*iter)->Update();

		if ((*iter)->IsDestory())
		{
			delete (*iter);
			iter = m_vecBubbleFlow.erase(iter);
		}
		else
			++iter;
	}
}

void CBubbleFlow_List::Render(void)
{
	for (size_t i = 0; i < m_vecBubbleFlow.size(); ++i)
	{
		if (m_vecBubbleFlow[i]->IsDestory() == false && m_vecBubbleFlow[i]->IsVisible())
		{
			m_vecBubbleFlow[i]->Render();
		}
	}
}


CBubbleFlow_List * CBubbleFlow_List::Create(VECTOR2 vCenter, int iPower, E_FLOW_STATE eCenterFlow)
{
	CBubbleFlow_List* pList = new CBubbleFlow_List(vCenter, iPower, eCenterFlow);
	pList->Initialize();

	return pList;
}

void CBubbleFlow_List::DestoryFlow(CBubbleFlow* pBubble, int iOffsetIndex)
{
	if (pBubble == NULL || pBubble->IsDestory())return;

	const char* szKey = NULL;
	int iIndex = -1;

	for (size_t i = 0; i < m_vecBubbleFlow.size(); ++i)
	{
		if (pBubble == m_vecBubbleFlow[i])
		{
			iIndex = i + iOffsetIndex;
			break;
		}
	}

	if (iIndex == -1 || m_vecBubbleFlow.size() <= iIndex)return;
	szKey = pBubble->GetKey();

	for (int i = iIndex; ; ++i)
	{
		if (m_vecBubbleFlow.size() <= i
			|| strcmp(szKey, m_vecBubbleFlow[i]->GetKey()) != 0)break;

		m_vecBubbleFlow[i]->SetVisible(false);
		m_vecBubbleFlow[i]->SetDestroy();
	}
}
