#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene_Login.h"
#include "BitmapMgr.h"
#include "Scene_Lobby.h"
//#include "Scene_Ingame.h"
//#include "Scene_TileEdit.h"
//#include "Scene_ObjectEdit.h"
#include "Scene_Test.h"
IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
{
	m_ePrevScene = m_eSceneType = E_SCENE_TYPE_END;
}


CSceneMgr::~CSceneMgr()
{
}

void CSceneMgr::ChangeScene(void)
{
	if (m_ePrevScene != m_eSceneType)
	{
		SetScene(m_eSceneType);
	}
}

void CSceneMgr::SetScene(E_SCENE_TYPE eSceneType)
{
	m_eSceneType = eSceneType;
	m_ePrevScene = m_eSceneType;

	if (m_pScene)
	{
		delete m_pScene;
		m_pScene = NULL;
	}

	switch (eSceneType)
	{
	case E_SCENE_TYPE_LOGIN:
		m_pScene = CScene_Login::Create();
		CBitmapMgr::GetInstance()->LoadImageByScene(eSceneType);
		break;
	case E_SCENE_TYPE_LOBBY:
		m_pScene = CScene_Lobby::Create();
		CBitmapMgr::GetInstance()->LoadImageByScene(eSceneType);
		break;
	case E_SCENE_TYPE_INGAME:
		//m_pScene = CScene_Ingame::Create();
		CBitmapMgr::GetInstance()->LoadImageByScene(eSceneType);
		break;
	case E_SCENE_TYPE_TILE_EDIT:
		//m_pScene = CScene_TileEdit::Create();
		CBitmapMgr::GetInstance()->LoadImageByScene(eSceneType);
		break;
	case E_SCENE_TYPE_OBJECT_EDIT:
	//	m_pScene = CScene_ObjectEdit::Create();
		CBitmapMgr::GetInstance()->LoadImageByScene(eSceneType);
		break;
	case E_SCENE_TYPE_TEST:
		m_pScene = CScene_Test::Create();
		CBitmapMgr::GetInstance()->LoadImageByScene(eSceneType);
		break;
	}
}

void CSceneMgr::Update(void)
{
	//WaitForSingleObject(g_hMutex, INFINITE);
	SAFE_FUNC(m_pScene, Update());
	//ReleaseMutex(g_hMutex);
}

void CSceneMgr::Render(void)
{
	//WaitForSingleObject(g_hMutex, INFINITE);
	SAFE_FUNC(m_pScene, Render());
	//ReleaseMutex(g_hMutex);
}
