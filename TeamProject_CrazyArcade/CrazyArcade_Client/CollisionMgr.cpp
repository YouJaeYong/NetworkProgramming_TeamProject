#include "stdafx.h"
#include "CollisionMgr.h"
#include "MouseMgr.h"
#include "Object.h"
#include "Block.h"
#include "MapMgr.h"
#include "ObjectMgr.h"
#include "BubbleFlow_List.h"
#include "BubbleFlow.h"
#include "Bubble.h"
#include "Character.h"
#include "Item.h"
#include"SoundMgr.h"
IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::CollMouseToRect(RECT rc)
{
	POINT pt = CMouseMgr::GetInstance()->GetCurPos();

	if (PtInRect(&rc, pt))
	{
		return true;
	}
	return false;
}
bool CCollisionMgr::ScreenOutCheck(VECTOR2 vStart, VECTOR2 vDest, VECTOR2* vOut, RECT rcScreen)
{
	bool IsOut = false;
	if (vDest.x < rcScreen.left || vDest.x > rcScreen.right
		|| vDest.y < rcScreen.top || vDest.y > rcScreen.bottom)
	{
		IsOut = true;
	}
	vDest.x = max(rcScreen.left, vDest.x);
	vDest.x = min(rcScreen.right, vDest.x);

	vDest.y = max(rcScreen.top, vDest.y);
	vDest.y = min(rcScreen.bottom, vDest.y);


	*vOut = vDest;
	return IsOut;
}
bool CCollisionMgr::CollisionObjectToBlock(CObject * pObj, VECTOR2 vDest, VECTOR2 * vOut)
{
	if (pObj == NULL)return false;

	VECTOR2 vPrevPos = pObj->GetInfo()->vPos;

	vector<CBlock*> vecBlock = *CMapMgr::GetInstance()->GetVecBlock();
	for (size_t i = 0; i < vecBlock.size(); ++i)
	{
		RECT rc;
		pObj->GetInfo()->vPos = vDest;
		pObj->CalcRect();//이동 후 렉트 계산

		if (IntersectRect(&rc, &vecBlock[i]->GetInfo()->rcRect, &pObj->GetInfo()->rcRect))
		{
			if (vecBlock[i]->GetBlockType() == E_BLOCK_TYPE_BUSH)
			{
				pObj->SetVisible(!pObj->IsVisible());
				vecBlock[i]->SetBushIsIn(pObj);
				*vOut = vDest;
				continue;
			}

			//벽돌과 충돌 보정
			VECTOR2 vCenter;
			VECTOR2	vOffset;
			vOffset.x = rc.right - rc.left;
			vOffset.y = rc.bottom - rc.top;


			vCenter.x = long((rc.right + rc.left) / 2);
			vCenter.y = long((rc.bottom + rc.top) / 2);



			if (vOffset.x >= vOffset.y)vOffset.x = 0.15f;
			else vOffset.y = 0.15f;



			if (vCenter.x > pObj->GetInfo()->vPos.x)vOffset.x *= -1;
			if (vCenter.y > pObj->GetInfo()->vPos.y + pObj->GetOffset().y)vOffset.y *= -1;

			//충돌 체크 
			if (CCollisionMgr::GetInstance()->CheckObjectToBlock(pObj, pObj->GetInfo()->vPos + vOffset))
			{
				//충돌됨
				if (vOffset.x > 0 && vOffset.x < 0.2f)vOffset.x = 0.0f;
				else if (vOffset.y > 0 && vOffset.y < 0.2f)vOffset.y = 0.0f;
			}

			*vOut = pObj->GetInfo()->vPos + vOffset;
			pObj->GetInfo()->vPos = vPrevPos;
			return true;
		}
	}
	pObj->GetInfo()->vPos = vPrevPos;
	*vOut = vDest;
	return false;
}

bool CCollisionMgr::CheckObjectToBlock(CObject * pObj, VECTOR2 vDest)
{
	vector<CBlock*> vecBlock = *CMapMgr::GetInstance()->GetVecBlock();
	VECTOR2 vPrevPos = pObj->GetInfo()->vPos;
	pObj->GetInfo()->vPos = vDest;
	pObj->CalcRect();//이동 후 렉트 계산

	for (size_t i = 0; i < vecBlock.size(); ++i)
	{
		RECT rc;
		if (IntersectRect(&rc, &pObj->GetInfo()->rcRect, &vecBlock[i]->GetInfo()->rcRect))
		{
			pObj->GetInfo()->vPos = vPrevPos;
			return true;
		}
	}
	pObj->GetInfo()->vPos = vPrevPos;
	return false;
}

bool CCollisionMgr::CollisionObjectToBubble(CObject* pObj, VECTOR2 vDest, VECTOR2* vOut)
{
	if (pObj == NULL)return false;
	VECTOR2 vPrevPos = pObj->GetInfo()->vPos;


	vector<CObject*> vecObject = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_BUBBLE);
	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		if (vecObject[i] == pObj)
			continue;
		CBubble* pBubble = dynamic_cast<CBubble*>(vecObject[i]);
		vector<CObject*>		vecNotReady = pBubble->GetVecNotReady();
		vector<CObject*>::iterator iter = find(vecNotReady.begin(), vecNotReady.end(), pObj);

		if (iter != vecNotReady.end())
		{
			*vOut = vDest;
			return false;
		}
		RECT rc;
		pObj->GetInfo()->vPos = vDest;
		pObj->CalcRect();//이동 후 렉트 계산
		if (IntersectRect(&rc, &vecObject[i]->GetInfo()->rcRect, &pObj->GetInfo()->rcRect))
		{
			VECTOR2 vCenter;
			VECTOR2	vOffset;
			vOffset.x = rc.right - rc.left;
			vOffset.y = rc.bottom - rc.top;


			vCenter.x = long((rc.right + rc.left) / 2);
			vCenter.y = long((rc.bottom + rc.top) / 2);

			if (vOffset.x >= vOffset.y)vOffset.x = 0;
			else vOffset.y = 0;

			if (vCenter.x > pObj->GetInfo()->vPos.x)
				vOffset.x *= -1;
			if (vCenter.y > pObj->GetInfo()->vPos.y + pObj->GetOffset().y)
				vOffset.y *= -1;

			*vOut = pObj->GetInfo()->vPos + vOffset;
			pObj->GetInfo()->vPos = vPrevPos;

			CCharacter* pCharacter = dynamic_cast<CCharacter*>(pObj);

			if (pCharacter)
			{
				pCharacter->Kick(pBubble);
				if (pBubble->IsKick() == true)
				{
					pBubble->SetKick(false, VECTOR2(0, 0));
				}
			}

			return true;

		}
	}

	pObj->GetInfo()->vPos = vPrevPos;
	return false;
}

bool CCollisionMgr::CheckFlowToBlock(CBubbleFlow_List* pFlowList)
{
	if (pFlowList == NULL)return false;

	vector<CBlock*>		vecBlock = *CMapMgr::GetInstance()->GetVecBlock();
	vector<CBubbleFlow*> vecFlow = *pFlowList->GetVecFlow();

	for (size_t j = 0; j < vecFlow.size(); ++j)
	{
		if (vecFlow[j]->IsDestory() || vecFlow[j]->IsVisible() == false)
			continue;

		for (size_t i = 0; i < vecBlock.size(); ++i)
		{

			RECT rc;
			if (IntersectRect(&rc, &vecFlow[j]->GetInfo()->rcRect, &vecBlock[i]->GetInfo()->rcRect))
			{

				if (vecBlock[i]->GetBlockType() == E_BLOCK_TYPE_STATIC)
				{
					pFlowList->DestoryFlow(vecFlow[j], 0);

				}
				else if (vecBlock[i]->GetBlockType() == E_BLOCK_TYPE_BUSH)
				{
					vecBlock[i]->SetDestroy();
				}
				else
				{
					//블럭없어진다.
					vecBlock[i]->SetPlayAni(true);
					pFlowList->DestoryFlow(vecFlow[j], 1);
				}

				break;
			}
		}
	}
	return false;
}

bool CCollisionMgr::CheckFlowToBubble(CBubbleFlow_List* pFlowList)
{
	if (pFlowList == NULL)return false;


	vector<CObject*> vecBubble = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_BUBBLE);

	for (size_t i = 0; i < vecBubble.size(); ++i)
	{
		CBubble* pBubble = dynamic_cast<CBubble*>(vecBubble[i]);
		if (pBubble && pBubble->IsCreateFlow())continue;
		vector<CBubbleFlow*> vecFlow = *pFlowList->GetVecFlow();

		for (int j = 0; j < vecFlow.size(); ++j)
		{
			RECT rc;
			if (IntersectRect(&rc, &vecFlow[j]->GetInfo()->rcRect, &pBubble->GetInfo()->rcRect))
			{
				if (vecFlow[j]->IsVisible() == false || vecFlow[j]->IsDestory())
				{
					continue;
				}
				if (pBubble)
				{
					pBubble->CreateBubbleFlow();
				}
			}
		}
	}


	return false;
}

bool CCollisionMgr::CheckFlowToPlayer(CBubbleFlow_List* pFlowList)
{
	if (pFlowList == NULL)return false;

	vector<CBubbleFlow*> vecBubbleFlow = *pFlowList->GetVecFlow();

	vector<CObject*>	vecPlayer = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_PLAYER);


	for (size_t i = 0; i < vecBubbleFlow.size(); ++i)
	{
		if (vecBubbleFlow[i]->IsDestory() || vecBubbleFlow[i]->IsVisible() == false)
			continue;

		for (size_t j = 0; j < vecPlayer.size(); ++j)
		{

			RECT rc;
			if (IntersectRect(&rc, &vecBubbleFlow[i]->GetInfo()->rcRect, &vecPlayer[j]->GetInfo()->rcRect))
			{
				CCharacter* pCharacter = dynamic_cast<CCharacter*>(vecPlayer[j]);
				if (pCharacter)
				{
					pCharacter->CollisionFlow();//충돌처리
					continue;
				}
			}
		}
	}
	return false;
}

bool CCollisionMgr::CollisionPosToObject(VECTOR2 vPos, E_OBJ_LIST_TYPE _eType)
{
	if (_eType >= E_OBJ_LIST_END)return false;

	vector<CObject*>		vecObject = *CObjectMgr::GetInstance()->GetVecObject(_eType);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		POINT pt;
		pt.x = long(vPos.x);
		pt.y = long(vPos.y);

		if (PtInRect(&vecObject[i]->GetInfo()->rcRect, pt))
		{
			return true;
		}
	}
	return false;
}

bool CCollisionMgr::CheckFlowToWindow(CBubbleFlow_List* pFlowList)
{
	if (pFlowList == NULL)return false;

	vector<CBubbleFlow*>	vecFlow = *pFlowList->GetVecFlow();

	for (size_t i = 0; i < vecFlow.size(); ++i)
	{
		if (CheckRectToWindow(vecFlow[i]->GetInfo()->rcRect))
		{
			pFlowList->DestoryFlow(vecFlow[i]);
		}
	}
	return false;
}

bool CCollisionMgr::CollisionPlayerToItem(CCharacter* pCharacter)
{
	if (pCharacter == NULL || pCharacter->IsDestory())return false;
	if (pCharacter->GetState() >= E_CHARACTER_STATE_DROWNING)return false;

	vector<CObject*>		vecItem = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_ITEM);

	for (int i = 0; i < vecItem.size(); ++i)
	{
		RECT rc;
		if (IntersectRect(&rc, &pCharacter->GetInfo()->rcRect, &vecItem[i]->GetInfo()->rcRect))
		{
			CItem* pItem = dynamic_cast<CItem*>(vecItem[i]);

			E_ITEMTYPE m_eItemType = pItem->GetItemType();

			if (pItem)
			{
				if (m_eItemType <= E_ITEMTYPE_KICK)
				{
					CSoundMgr::GetInstance()->PlayEnviroment(L"SFX_Item_Off.wav", g_fVolume);

					pCharacter->GetCharacterInfo()->EatItem(m_eItemType);
					vecItem[i]->SetDestroy();
					vecItem[i]->SetVisible(false);
					return true;
				}
				else if (m_eItemType == E_ITEMTYPE_BANANA)
				{
					if (pItem->GetOwner() != pCharacter)
					{
						pItem->SetVisible(false);
						pItem->SetDestroy();

						//미끄러진다
						pCharacter->Sliding();
					}
				}
			}
		}
	}

	return false;
}

bool CCollisionMgr::CollisionPlayerToPlayer(CCharacter* pCharacter)
{
	if (pCharacter == NULL || pCharacter->IsVisible() == false || pCharacter->GetState() != E_CHARACTER_STATE_DROWNING)return false;

	vector<CObject*> vecObject = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_PLAYER);

	for (size_t i = 0; i < vecObject.size(); ++i)
	{
		CCharacter* pDest = dynamic_cast<CCharacter*>(vecObject[i]);
		if (pCharacter == pDest)
			continue;

		RECT rc;
		if (IntersectRect(&rc, &pCharacter->GetInfo()->rcRect, &pDest->GetInfo()->rcRect))
		{
			if (pDest)
			{
				if (pDest->IsDestory() || pDest->GetState() >= E_CHARACTER_STATE_DROWNING)
					continue;

				if (pCharacter->GetCharacterType() == pDest->GetCharacterType())
				{
					pCharacter->Revive();//부활
					return true;
				}
				else
				{
					pCharacter->Dead();//죽음
					return true;
				}

			}
		}

	}
	return false;
}

bool CCollisionMgr::CollisionBubbleToItem(CBubble * pBubble)
{
	if (pBubble == NULL || pBubble->IsDestory())return false;

	vector<CObject*> vecItem = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_ITEM);

	for (size_t i = 0; i < vecItem.size(); ++i)
	{
		CItem* pItme = dynamic_cast<CItem*>(vecItem[i]);

		if (pItme)
		{
			RECT rc;
			if (IntersectRect(&rc, &pBubble->GetInfo()->rcRect, &pItme->GetInfo()->rcRect))
			{
				if (pItme->GetItemType() == E_ITEMTYPE_DART || pItme->GetItemType() == E_ITEMTYPE_MINE)
				{
					pItme->SetDestroy();
					pBubble->CreateBubbleFlow();
				}
			}

		}
	}

	return false;
}
bool CCollisionMgr::CheckFlowToItem(CBubbleFlow_List * pFlowList)
{
	if (pFlowList == NULL)return false;

	vector<CBubbleFlow*> vecBubbleFlow = *pFlowList->GetVecFlow();
	vector<CObject*> vecItem = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_ITEM);

	for (size_t i = 0; i < vecBubbleFlow.size(); ++i)
	{
		if (vecBubbleFlow[i]->IsDestory() || vecBubbleFlow[i]->IsVisible() == false) continue;

		for (size_t j = 0; j < vecItem.size(); ++j)
		{
			RECT rc;
			if (IntersectRect(&rc, &vecItem[j]->GetInfo()->rcRect, &vecBubbleFlow[i]->GetInfo()->rcRect))
			{
				CItem* pItem = dynamic_cast<CItem*>(vecItem[j]);
				if (pItem->GetItemType() != E_ITEMTYPE_DART)
				{
					vecItem[j]->SetVisible(false);
					vecItem[j]->SetDestroy();
					continue;
				}
			}
		}

	}
	return false;
}
bool CCollisionMgr::CheckRectToWindow(RECT rc)
{
	RECT rcScreen;

	rcScreen.left = long(20);
	rcScreen.right = long(630);

	rcScreen.top = long(30);
	rcScreen.bottom = long(560);

	if (rc.left < rcScreen.left || rc.right > rcScreen.right
		|| rc.top < rcScreen.top || rc.bottom > rcScreen.bottom)return true;


	return false;
}
