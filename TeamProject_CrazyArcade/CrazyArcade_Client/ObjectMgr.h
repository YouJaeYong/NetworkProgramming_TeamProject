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
	void Update(void);
	void Render(void);


public:
	void AddObject(CObject* pObject);
	void AddRenderer(CObject* pObject);
	vector<CObject*>* GetVecObject(E_OBJ_LIST_TYPE _eType) { if (_eType >= E_OBJ_LIST_END) return NULL;  return &m_vecObject[_eType]; }
	void DeleteRender(CObject* pObject);

	CObject* FindObject(int iID);
	CObject* FindObject(int iobjListType, int iID);
	void ServerCreateObject(CObject* pobj);
	void ServerRemoveObject(CObject* pobj);
	void Clear(void);
private:
	static bool YSort(CObject* pDst, CObject* pSrc);
	void Release(void);

private:
	vector<CObject*>						m_vecObject[E_OBJ_LIST_END];
	vector<CObject*>						m_vecRenderer[E_RENDER_TYPE_END];
	queue<CObject*>							m_qCreateObject;
	queue<CObject*>							m_qRemoveObject;
};

