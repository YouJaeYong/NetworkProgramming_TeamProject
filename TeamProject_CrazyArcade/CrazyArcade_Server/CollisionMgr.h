#pragma once
#include "Include.h"

class CCharacter_Server;

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	CCollisionMgr();
	~CCollisionMgr();
public:
	bool CollisionObjectToBlock(CCharacter_Server* pObj, VECTOR2 vDest, VECTOR2* vOut);
	bool CheckObjectToBlock(CCharacter_Server * pObj, VECTOR2 vDest);
};

