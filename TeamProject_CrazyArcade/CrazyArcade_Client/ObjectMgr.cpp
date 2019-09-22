#include "stdafx.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "MapMgr.h"
#include "Block.h"
#include "Tile.h"


IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{

}


CObjectMgr::~CObjectMgr()
{
	Release();
}

void CObjectMgr::Update(void)
{
	//데이터 추가
	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < m_qCreateObject.size(); ++i)
	{
		AddObject(m_qCreateObject.front());
		m_qCreateObject.pop();
	}
	for (size_t i = 0; i < m_qRemoveObject.size(); ++i)
	{
		m_qRemoveObject.front()->SetDestroy();//삭제
		m_qRemoveObject.pop();
	}
	LeaveCriticalSection(&g_cs);
	for (int i = 0; i < E_OBJ_LIST_END; ++i)
	{
		vector<CObject*>::iterator iterObj = m_vecObject[i].begin();
		vector<CObject*>::iterator iteObj_end = m_vecObject[i].end();

		for (iterObj; iterObj != m_vecObject[i].end(); )
		{
			(*iterObj)->Update();

			if ((*iterObj)->IsDestory())
			{
				delete (*iterObj);
				iterObj = m_vecObject[i].erase(iterObj);
			}
			else
				++iterObj;
		}
	}


	vector<CBlock*>* vecBlock = CMapMgr::GetInstance()->GetVecBlock();

	vector<CBlock*>::iterator iterBlock = vecBlock->begin();
	vector<CBlock*>::iterator iteBlock_end = vecBlock->end();


	for (iterBlock; iterBlock != vecBlock->end(); )
	{
		(*iterBlock)->Update();

		if ((*iterBlock)->IsDestory())
		{
			delete (*iterBlock);
			iterBlock = vecBlock->erase(iterBlock);
		}
		else
			++iterBlock;
	}
}

void CObjectMgr::Render(void)
{

	for (int i = 0; i < E_RENDER_TYPE_END; ++i)
	{
		if (i == E_RENDER_TYPE_SORT_OBJECT)
		{
			sort(m_vecRenderer[i].begin(), m_vecRenderer[i].end(), YSort);
		}
		for (size_t j = 0; j < m_vecRenderer[i].size(); ++j)
		{
			m_vecRenderer[i][j]->Render();
		}
	}
}

void CObjectMgr::AddObject(CObject * pObject)
{
	if (pObject == NULL)
		return;
	CBlock* pBlock = dynamic_cast<CBlock*>(pObject);
	CTile*	pTile = dynamic_cast<CTile*>(pObject);

	if (pBlock)
	{
		CMapMgr::GetInstance()->AddBlock(pBlock);
		return;
	}
	else if (pTile)
	{
		CMapMgr::GetInstance()->AddTile(pTile);
		return;
	}

	if (pObject->GetObjListType() >= E_OBJ_LIST_END || pObject->GetRenderType() >= E_RENDER_TYPE_END)
		return;

	m_vecObject[pObject->GetObjListType()].push_back(pObject);
	m_vecRenderer[pObject->GetRenderType()].push_back(pObject);

	//if (pObject == NULL || pObject->GetObjListType() >= E_OBJ_LIST_END || pObject->GetRenderType() >= E_RENDER_TYPE_END)return;
	//m_vecObject[pObject->GetObjListType()].push_back(pObject);
	//m_vecRenderer[pObject->GetRenderType()].push_back(pObject);
}

void CObjectMgr::AddRenderer(CObject * pObject)
{
	if (pObject == NULL || pObject->GetRenderType() >= E_RENDER_TYPE_END)return;
	m_vecRenderer[pObject->GetRenderType()].push_back(pObject);

}

void CObjectMgr::DeleteRender(CObject* pObject)
{
	if (pObject == NULL || pObject->GetRenderType() >= E_RENDER_TYPE_END)return;

	vector<CObject*>::iterator iter = m_vecRenderer[pObject->GetRenderType()].begin();
	vector<CObject*>::iterator iter_end = m_vecRenderer[pObject->GetRenderType()].end();

	for (iter; iter != iter_end; ++iter)
	{
		if (*iter == pObject)
		{
			m_vecRenderer[pObject->GetRenderType()].erase(iter);
			return;
		}
	}
}

CObject * CObjectMgr::FindObject(int iID)
{
	//vector<CObject*>						m_vecObject[E_OBJ_LIST_END];
	for (int i = 0; i < E_OBJ_LIST_END; ++i)
	{
		if (i == E_OBJ_LIST_TYPE_PLAYER)
			continue;

		for (size_t j = 0; j < m_vecObject[i].size(); ++j)
		{
			if (m_vecObject[i][j]->IsDestory() == false && m_vecObject[i][j]->GetID() == iID)
				return  m_vecObject[i][j];
		}
	}


	vector<CBlock*>	vecBlock = *CMapMgr::GetInstance()->GetVecBlock();
	for (size_t j = 0; j < vecBlock.size(); ++j)
	{
		if (vecBlock[j]->IsDestory() == false && vecBlock[j]->GetID() == iID)
			return  vecBlock[j];
	}

	return NULL;
}

CObject* CObjectMgr::FindObject(int iobjListType, int iID)
{
	if (iobjListType >= E_OBJ_LIST_END)
		return NULL;
	for (size_t j = 0; j < m_vecObject[iobjListType].size(); ++j)
	{
		if (m_vecObject[iobjListType][j]->IsDestory() == false && m_vecObject[iobjListType][j]->GetID() == iID)
			return  m_vecObject[iobjListType][j];
	}
	return NULL;
}

void CObjectMgr::ServerCreateObject(CObject* pobj)
{
	if (pobj)
		m_qCreateObject.push(pobj);
}

void CObjectMgr::ServerRemoveObject(CObject* pobj)
{
	if (pobj)
		m_qRemoveObject.push(pobj);
}

void CObjectMgr::Clear(void)
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

bool CObjectMgr::YSort(CObject * pDst, CObject * pSrc)
{

	if (pSrc->GetInfo()->vPos.y > pDst->GetInfo()->vPos.y)return true;
	return false;

}

void CObjectMgr::Release(void)
{
	for (int i = 0; i < E_OBJ_LIST_END; ++i)
	{
		for (size_t j = 0; j < m_vecObject[i].size(); ++j)
		{
			delete m_vecObject[i][j];
		}
		m_vecObject[i].clear();
	}


	for (int i = 0; i < E_RENDER_TYPE_END; ++i)
	{
		m_vecRenderer[i].clear();
	}

}
