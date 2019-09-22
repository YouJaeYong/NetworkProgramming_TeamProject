#include "stdafx.h"
#include "MainGame.h"
#include "Include.h"
#include "SceneMgr.h"
#include "BitmapMgr.h"

#include "ClientMgr.h"
#include "KeyMgr.h"
#include "MouseMgr.h"
#include "TimeMgr.h"
#include "NetMgr.h"
#include "MapMgr.h"
#include "ObjectMgr.h"
#include "GameMgr.h"
#include "CollisionMgr.h"

#include"SoundMgr.h"
#ifdef __DEBUG__
#pragma warning(disable: 4996)
#endif
#if (_MSC_VER >= 1915)
#pragma warning(disable:4845)   // __declspec(no_init_all) used but d1initall not set
#endif
CMainGame::CMainGame()
{
	srand(unsigned(time(NULL)));
	g_HDC = GetDC(g_hWnd);
	InitializeCriticalSection(&g_cs);//임계영역 초기화
}


CMainGame::~CMainGame()
{
	Release();

}
void CMainGame::Initialize(void)
{
	CSoundMgr::GetInstance()->Initialize();
	CTimeMgr::GetInstance()->InitTime();
	CSceneMgr::GetInstance()->SetScene(E_SCENE_TYPE_LOGIN);
}
void CMainGame::Update(void)
{
	CMouseMgr::GetInstance()->Update();
	CTimeMgr::GetInstance()->SetTime();
	CGameMgr::GetInstance()->Update();
	CSceneMgr::GetInstance()->Update();
	CSoundMgr::GetInstance()->Update();
}
void CMainGame::Render(void)
{//
	CSceneMgr::GetInstance()->Render();
	BitBlt(g_HDC,		//복사될 대상 DC
		0,				//복사될 좌표 위치x 
		0,				//복사될 좌표 위치 y
		WINCX,			// 이미지 크기 X
		WINCY,			// 이미지 크기
		CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"),//복사할 DC
		0, //이미지 어디부터 출력할 것인가?
		0, //이미지 어디부터 출력할 것인가?
		SRCCOPY);
}


void CMainGame::Release(void)
{
	ReleaseDC(g_hWnd, g_HDC);
	CKeyMgr::GetInstance()->DestroyInstance();
	CMouseMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CObjectMgr::GetInstance()->DestroyInstance();
	CBitmapMgr::GetInstance()->DestroyInstance();
	CGameMgr::GetInstance()->DestroyInstance();
	CNetMgr::GetInstance()->DestroyInstance();
	CClientMgr::GetInstance()->DestroyInstance();
	CMapMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
}
