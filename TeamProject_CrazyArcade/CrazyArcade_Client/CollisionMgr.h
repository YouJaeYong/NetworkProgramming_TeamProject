#pragma once
#include "Include.h"

class CObject;
class CBubbleFlow_List;
class CCharacter;
class CBubble;
class CBubbleFlow_List;

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	CCollisionMgr();
	~CCollisionMgr();
public:
	bool CollMouseToRect(RECT rc);
	bool ScreenOutCheck(VECTOR2 vStart, VECTOR2 vDest, VECTOR2* vOut, RECT rcScreen);
	bool CollisionObjectToBlock(CObject* pObj, VECTOR2 vDest, VECTOR2* vOut);
	bool CheckObjectToBlock(CObject * pObj, VECTOR2 vDest);
	bool CollisionObjectToBubble(CObject* pObj, VECTOR2 vDest, VECTOR2* vOut);


	bool CheckFlowToBlock(CBubbleFlow_List* pFlowList);
	bool CheckFlowToBubble(CBubbleFlow_List* pFlowList);
	bool CheckFlowToPlayer(CBubbleFlow_List* pFlowList);
	bool CollisionPosToObject(VECTOR2 vPos, E_OBJ_LIST_TYPE _eType);
	bool CheckFlowToWindow(CBubbleFlow_List* pFlowList);


	bool CollisionPlayerToItem(CCharacter* pCharacter);
	bool CollisionPlayerToPlayer(CCharacter* pCharacter);


	bool CollisionBubbleToItem(CBubble * pBubble);
	bool CheckFlowToItem(CBubbleFlow_List * pFlowList);
private:
	bool CheckRectToWindow(RECT rc);
};

