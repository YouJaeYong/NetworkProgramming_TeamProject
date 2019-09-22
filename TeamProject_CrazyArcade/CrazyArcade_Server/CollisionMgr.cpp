#include "CollisionMgr.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "Block.h"
#include "Character_Server.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::CollisionObjectToBlock(CCharacter_Server* pObj, VECTOR2 vDest, VECTOR2* vOut)
{
	if (pObj == NULL)
		return false;

	pObj->GetInfo()->vPos = *pObj->GetVecPrevPos();
	VECTOR2 vPrevPos = pObj->GetInfo()->vPos;

	vector<CObject*> vecBlock = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_BLOCK);
	for (size_t i = 0; i < vecBlock.size(); ++i)
	{
		CBlock*		pBlock = dynamic_cast<CBlock*>(vecBlock[i]);
		if (pBlock == NULL)
			continue;

		RECT rc;
		pObj->GetInfo()->vPos = vDest;
		pObj->CalcRect();//이동 후 렉트 계산

		if (IntersectRect(&rc, &pBlock->GetRect(), &pObj->GetRect()))
		{
			if (pBlock->GetBlockType() == E_BLOCK_TYPE_BUSH)
			{
				//해당 아이디 부쉬에 들어감
				//pObj->SetVisible(!pObj->IsVisible());
				//	vecBlock[i]->SetBushIsIn(pObj);
				*vOut = vDest;
				continue;
			}

			//벽돌과 충돌 보정
			VECTOR2 vCenter;
			VECTOR2	vOffset;
			vOffset.x = (float)(rc.right - rc.left);
			vOffset.y = (float)(rc.bottom - rc.top);


			vCenter.x = float((rc.right + rc.left) / 2);
			vCenter.y = float((rc.bottom + rc.top) / 2);



			if (vOffset.x >= vOffset.y)
				vOffset.x = 0.15f;
			else vOffset.y = 0.15f;



			if (vCenter.x > pObj->GetInfo()->vPos.x)
				vOffset.x *= -1;
			if (vCenter.y > pObj->GetInfo()->vPos.y + 20)
				vOffset.y *= -1;

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

bool CCollisionMgr::CheckObjectToBlock(CCharacter_Server * pObj, VECTOR2 vDest)
{
	vector<CObject*> vecBlock = *CObjectMgr::GetInstance()->GetVecObject(E_OBJ_LIST_TYPE_BLOCK);
	VECTOR2 vPrevPos = pObj->GetInfo()->vPos;
	pObj->GetInfo()->vPos = vDest;
	pObj->CalcRect();//이동 후 렉트 계산

	for (size_t i = 0; i < vecBlock.size(); ++i)
	{
		RECT rc;
		if (IntersectRect(&rc, &pObj->GetRect(), &vecBlock[i]->GetRect()))
		{
			pObj->GetInfo()->vPos = vPrevPos;
			return true;
		}
	}
	pObj->GetInfo()->vPos = vPrevPos;
	return false;
}
