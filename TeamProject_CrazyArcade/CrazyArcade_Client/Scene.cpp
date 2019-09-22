#include "stdafx.h"
#include "Scene.h"
#include "UI.h"

CScene::CScene()
{
}


CScene::~CScene()
{
	Release();
}
void CScene::AddUI(CUI * pUI, const char* pKey)
{
	pUI->SetKey(pKey);
	m_vecUI.push_back(pUI);
}
void CScene::UISortByLevel(void)
{
	sort(m_vecUI.begin(), m_vecUI.end(), SortUI);
}

void CScene::UIUpdate(void)
{
	vector<CUI*>::iterator iter = m_vecUI.begin();
	vector<CUI*>::iterator iter_end = m_vecUI.end();
	for (iter; iter != m_vecUI.end();)
	{
		(*iter)->Update();

		if ((*iter)->IsDestroy())
		{
			delete *iter;
			iter = m_vecUI.erase(iter);
		}
		else
			++iter;

	}

}

void CScene::UIRender(void)
{
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}
}

CUI * CScene::FindUI(const char * strKey)
{
	if (strcmp("", strKey) == 0)return NULL;

	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		if (strcmp(m_vecUI[i]->GetKey(), strKey) == 0)
			return m_vecUI[i];
	}
	return NULL;
}

bool CScene::SortUI(CUI * pDest, CUI * pSrc)
{
	if (pDest->GetUILevel() < pSrc->GetUILevel())return true;
	return false;
}

void CScene::Release(void)
{
	vector<CUI*>					m_vecUI;

	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		delete m_vecUI[i];
	}
	m_vecUI.clear();
}

