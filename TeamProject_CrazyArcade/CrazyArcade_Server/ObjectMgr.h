#pragma once
#include "Include.h"
class CObject;

class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)
private:
	CObjectMgr();
	~CObjectMgr();
public:
	void Initialize();
	void Update();

	void AddObject(int iType, CObject* pObj);
	vector<CObject*>* GetVecObject(int eType);
	bool RemoveObject(REMOVE_OBJECT_MSG& objmsg);

	void Clear();
private:
	void Release();
private:
	vector<CObject*>			m_vecObject[E_OBJ_LIST_END];//캐릭터는 User정보에 있음
	int							m_ObjectID[E_OBJ_LIST_END];//ObjectID
};

