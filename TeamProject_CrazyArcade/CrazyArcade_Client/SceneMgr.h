#pragma once
#include "Include.h"
class CScene;


class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void ChangeScene(void);
	void SetScene(E_SCENE_TYPE eSceneType);
	void Update(void);
	void Render(void);
	CScene* GetScene(void) { return m_pScene; }
	E_SCENE_TYPE GetSceneType(void) { return m_eSceneType; }
	void SetSceneType(E_SCENE_TYPE _eType) { m_eSceneType = _eType; }
private:
	CScene*				m_pScene;
	E_SCENE_TYPE		m_eSceneType;
	E_SCENE_TYPE		m_ePrevScene;
};

