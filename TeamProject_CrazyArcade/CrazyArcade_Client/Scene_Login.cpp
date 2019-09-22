#include "stdafx.h"
#include "Scene_Login.h"
#include "BitmapMgr.h"
#include "UI_Button.h"
#include "NetMgr.h"
#include "SoundMgr.h"

CScene_Login::CScene_Login()
{
	
}


CScene_Login::~CScene_Login()
{
	DestroyWindow(g_hEdit);
	DestroyWindow(g_hEdit2);
	//ShowWindow(g_hEdit, SW_HIDE);
	//ShowWindow(g_hEdit2, SW_HIDE);
}

void CScene_Login::Initialize(void)
{
	g_hEdit = CreateWindow(L"edit", L"",
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER,
		300, 410, 200, 20,
		g_hWnd, (HMENU)100, NULL, NULL);
	SendMessage(g_hEdit, EM_LIMITTEXT, (WPARAM)6, 0);

	g_hEdit2 = CreateWindow(L"edit", L"",
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER,
		300, 390, 200, 20,
		g_hWnd, (HMENU)100, NULL, NULL);
	SendMessage(g_hEdit2, EM_LIMITTEXT, (WPARAM)20, 0);
	//SetWindowText(g_hEdit2, L"10.10.108.21");
	SetWindowText(g_hEdit2, L"127.0.0.1");
	SetWindowText(g_hEdit2, L"121.139.68.228");

	AddUI(new CUI_Button(400, 455, IMAGE_INFO(L"Game_Start", 192, 55, RGB(0, 255, 0)) , E_BUTTON_TYPE_LOGIN, E_UI_LEVEL_1));

	//서버 연결
	/*char buf[12];
	_itoa_s(rand() % 100, buf, 10);
	CNetMgr::GetInstance()->InitNetWorkConnect("127.0.0.1", buf);*/
	UISortByLevel();

	CSoundMgr::GetInstance()->PlayBGM(L"BGM_StageLogin.wav", true, g_fVolume);
}

void CScene_Login::Update(void)
{
	UIUpdate();
	return ;
}

void CScene_Login::Render(void)
{
	BitBlt(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"),	//복사될 대상 DC
		0,			//복사될 좌표 위치X
		0,			//복사될 좌표 위치Y
		WINCX,		//이미지 크기 X
		WINCY,		//이미지 크기 Y
		CBitmapMgr::GetInstance()->GetMemDC(L"Logo"),	//복사할 DC
		0,	//이미지 어디부터 출력할 것인가?
		0,			//이미지 어디부터 출력할 것인가?
		SRCCOPY);

	UIRender();

	RECT rt = { 300, 410, 300 + 200, 410 + 20 };
	InvalidateRect(g_hWnd, &rt, false);

	rt = { 300, 390, 300 + 200, 390 + 20 };
	InvalidateRect(g_hWnd, &rt, false); //재용
}

CScene_Login * CScene_Login::Create(void)
{
	CScene_Login* pLogin = new CScene_Login;
	pLogin->Initialize();
	return pLogin;
}
