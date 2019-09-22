#include "ObjectMgr.h"
#include "Object.h"
IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

void CObjectMgr::Initialize()
{
	memset(m_ObjectID, 0, sizeof(int) * E_OBJ_LIST_END);
	for (int i = 0; i < E_OBJ_LIST_END; ++i)
	{
		for (size_t j = 0; j < m_vecObject[i].size(); ++j)
		{
			m_vecObject[i][j]->Initiailze();
		}
	}
}

void CObjectMgr::Update()
{
	for (int i = 0; i < E_OBJ_LIST_END; ++i)
	{
		for (size_t j = 0; j < m_vecObject[i].size(); ++j)
		{
			m_vecObject[i][j]->Update();
		}
	}
}

void CObjectMgr::AddObject(int iType, CObject* pObj)
{
	if (iType >= E_OBJ_LIST_END)
		return;
	pObj->SetID(m_ObjectID[iType]++);
	m_vecObject[iType].push_back(pObj);
}
vector<CObject*>* CObjectMgr::GetVecObject(int eType)
{
	if (eType >= E_OBJ_LIST_END)
		return NULL;
	return &m_vecObject[eType];
}
bool CObjectMgr::RemoveObject(REMOVE_OBJECT_MSG& objmsg)
{
	if (objmsg.iObjListType >= E_OBJ_LIST_END)
		return false;
	for (auto iter = m_vecObject[objmsg.iObjListType].begin(); iter != m_vecObject[objmsg.iObjListType].end(); ++iter)
	{
		if ((*iter)->GetID() == objmsg.iID)
		{

			delete (*iter);
			m_vecObject[objmsg.iObjListType].erase(iter);
			return true;
		}
	}
	return false;
}
void CObjectMgr::Clear()
{

	memset(m_ObjectID, 0, sizeof(int) * E_OBJ_LIST_END);
	//ID √ ±‚»≠
	Release();
}

void CObjectMgr::Release()
{
	for (int i = 0; i < E_OBJ_LIST_END; ++i)
	{
		for (size_t j = 0; j < m_vecObject[i].size(); ++j)
		{
			delete m_vecObject[i][j];
		}
		m_vecObject[i].clear();
	}
}
