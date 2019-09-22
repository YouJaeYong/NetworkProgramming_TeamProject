#include "stdafx.h"
#include "Scene_Lobby.h"
#include "Include.h"
#include "BitmapMgr.h"
#include "UI.h"
#include "Client.h"
#include "ClientMgr.h"
#include "UI_Font.h"
#include "ChatMgr.h"
#include "UI_Button.h"
#include "GameMgr.h"
#include "SoundMgr.h"

CScene_Lobby::CScene_Lobby()
{
	CGameMgr::GetInstance()->NewGame();
}


CScene_Lobby::~CScene_Lobby()
{
	DestroyWindow(g_hEdit);
}

void CScene_Lobby::Initialize(void)
{
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Prepare.wav", true, g_fVolume);

	CChatMgr::GetInstance()->ChatClear();

	AddUI(new CUI(115, 210, IMAGE_INFO(L"Lobby_Image_Empty", 158, 188, RGB(0, 255, 0)), E_UI_LEVEL_2));
	AddUI(new CUI(115 + 190, 210, IMAGE_INFO(L"Lobby_Image_Empty", 158, 188, RGB(0, 255, 0)), E_UI_LEVEL_2));
	AddUI(new CUI(115 + 190 * 2, 210, IMAGE_INFO(L"Lobby_Image_Empty", 158, 188, RGB(0, 255, 0)), E_UI_LEVEL_2));
	AddUI(new CUI(115 + 190 * 3, 210, IMAGE_INFO(L"Lobby_Image_Empty", 158, 188, RGB(0, 255, 0)), E_UI_LEVEL_2));


	AddUI(new CUI_Button(593, 513, IMAGE_INFO(L"Lobby_Button_Start", 192, 55, RGB(0, 255, 0)), E_BUTTON_TYPE_GAME_START, E_UI_LEVEL_2));

	


	g_hEdit = CreateWindow(L"edit", L"",
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER,
		140, 523, 253, 22,
		g_hWnd, (HMENU)100, NULL, NULL);
	SendMessage(g_hEdit, EM_LIMITTEXT, (WPARAM)40, 0);

	//WaitForSingleObject(g_hMutex, INFINITE);
	if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_END)
	{
		AddUI(new CUI(550, 405, IMAGE_INFO(L"Lobby_Map_0", 148, 128, RGB(1, 255, 2)), E_UI_LEVEL_2, false), "Lobby_Map");
		AddUI(new CUI_Button(697, 407 - 21 * 3, IMAGE_INFO(L"Lobby_Sel_0", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_1, E_UI_LEVEL_2, false), "Lobby_Sel_0");
		AddUI(new CUI_Button(697, 407 - 21 * 2, IMAGE_INFO(L"Lobby_Sel_1", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_2, E_UI_LEVEL_2, false), "Lobby_Sel_1");
		AddUI(new CUI_Button(697, 407 - 21 * 1, IMAGE_INFO(L"Lobby_Sel_2", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_3, E_UI_LEVEL_2, false), "Lobby_Sel_2");
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_1)
	{
		AddUI(new CUI(550, 405, IMAGE_INFO(L"Lobby_Map_0", 148, 128, RGB(1, 255, 2)), E_UI_LEVEL_2, true), "Lobby_Map");
		AddUI(new CUI_Button(697, 407 - 21 * 3, IMAGE_INFO(L"Lobby_Sel_0", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_1, E_UI_LEVEL_2, true), "Lobby_Sel_0");
		AddUI(new CUI_Button(697, 407 - 21 * 2, IMAGE_INFO(L"Lobby_Sel_1", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_2, E_UI_LEVEL_2, false), "Lobby_Sel_1");
		AddUI(new CUI_Button(697, 407 - 21 * 1, IMAGE_INFO(L"Lobby_Sel_2", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_3, E_UI_LEVEL_2, false), "Lobby_Sel_2");
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_2)
	{
		AddUI(new CUI(550, 405, IMAGE_INFO(L"Lobby_Map_1", 148, 128, RGB(1, 255, 2)), E_UI_LEVEL_2, true), "Lobby_Map");
		AddUI(new CUI_Button(697, 407 - 21 * 3, IMAGE_INFO(L"Lobby_Sel_0", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_1, E_UI_LEVEL_2, false), "Lobby_Sel_0");
		AddUI(new CUI_Button(697, 407 - 21 * 2, IMAGE_INFO(L"Lobby_Sel_1", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_2, E_UI_LEVEL_2, true), "Lobby_Sel_1");
		AddUI(new CUI_Button(697, 407 - 21 * 1, IMAGE_INFO(L"Lobby_Sel_2", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_3, E_UI_LEVEL_2, false), "Lobby_Sel_2");
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_3)
	{
		AddUI(new CUI(550, 405, IMAGE_INFO(L"Lobby_Map_2", 148, 128, RGB(1, 255, 2)), E_UI_LEVEL_2, true), "Lobby_Map");
		AddUI(new CUI_Button(697, 407 - 21 * 3, IMAGE_INFO(L"Lobby_Sel_0", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_1, E_UI_LEVEL_2, false), "Lobby_Sel_0");
		AddUI(new CUI_Button(697, 407 - 21 * 2, IMAGE_INFO(L"Lobby_Sel_1", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_2, E_UI_LEVEL_2, false), "Lobby_Sel_1");
		AddUI(new CUI_Button(697, 407 - 21 * 1, IMAGE_INFO(L"Lobby_Sel_2", 135, 21, RGB(0, 255, 0)), E_BUTTON_TYPE_MAP_3, E_UI_LEVEL_2, true), "Lobby_Sel_2");
	}
	
	InitClient();
//	ReleaseMutex(g_hMutex);

	UISortByLevel();

	//printf("로비\n");
}


void CScene_Lobby::Update(void)
{

	//printf("업데이트\n");
	//WaitForSingleObject(g_hMutex, INFINITE);
	UIUpdate();
	//ReleaseMutex(g_hMutex);
	return ;
}

void CScene_Lobby::Render(void)
{
	//printf("랜더\n");

	BitBlt(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"),	//복사될 대상 DC
		0,			//복사될 좌표 위치X
		0,			//복사될 좌표 위치Y
		WINCX,		//이미지 크기 X
		WINCY,		//이미지 크기 Y
		CBitmapMgr::GetInstance()->GetMemDC(L"Lobby"),	//복사할 DC
		0,	//이미지 어디부터 출력할 것인가?
		0,			//이미지 어디부터 출력할 것인가?
		SRCCOPY);

	////리스트
	////WaitForSingleObject(g_hMutex, INFINITE);
	UIRender();
	//ReleaseMutex(g_hMutex);

	CChatMgr::GetInstance()->RenderChatting(140, 500);

	RECT rt = { 140, 523, 140 + 253 ,523 + 22  };
	InvalidateRect(g_hWnd, &rt, false);
}

CScene_Lobby * CScene_Lobby::Create(void)
{
	CScene_Lobby* pLobby = new CScene_Lobby;
	pLobby->Initialize();
	return pLobby;
}

void CScene_Lobby::AddClient(CClient* pClient, int iIndex)
{
	if (pClient == NULL)return;

	switch (pClient->GetClientInfo()->cCharacter)
	{
	case E_CHARACTER_TYPE_DAO:
		AddUI(new CUI(114 + 190 * iIndex, 210 - 22, IMAGE_INFO(L"Lobby_Dao", 153, 101, RGB(0, 255, 0)), E_UI_LEVEL_2), pClient->GetID());
		break;
	case E_CHARACTER_TYPE_BAZZI:
		AddUI(new CUI(114 + 190 * iIndex, 210 - 22, IMAGE_INFO(L"Lobby_Bazzi", 153, 101, RGB(0, 255, 0)), E_UI_LEVEL_2), pClient->GetID());
		break;
	}
	if (pClient->GetClientInfo()->IsRoomMaster == true)
	{
		AddUI(new CUI_Font(150 + 190 * iIndex , 219, "반장", E_UI_LEVEL_3, RGB(255, 0, 0)));
	}
	else
	{
		char szKey[256];
		strcpy_s(szKey, sizeof(szKey), pClient->GetID());
		strcat_s(szKey, sizeof(szKey), "_Ready");
		if (pClient->GetClientInfo()->IsReady)
			AddUI(new CUI_Font(150 + 190 * iIndex, 219, "준비", E_UI_LEVEL_3, RGB(255, 255, 0) ,L"Arial" , true) , szKey);
		else
			AddUI(new CUI_Font(150 + 190 * iIndex, 219, "준비", E_UI_LEVEL_3, RGB(255, 255, 0), L"Arial", false), szKey);
	}
	AddUI(new CUI_Font(100 + 190 * iIndex, 280, pClient->GetID(), E_UI_LEVEL_3, RGB(255, 255, 0)));
}

void CScene_Lobby::InitClient(void)
{
	vector<CClient*>	vecClient = *CClientMgr::GetInstance()->GetClient();

	for (size_t i = 0; i < vecClient.size(); ++i)
	{
		switch (vecClient[i]->GetClientInfo()->cCharacter)
		{
		case E_CHARACTER_TYPE_DAO:
			AddUI(new CUI(114 + 190 * i, 210 - 22, IMAGE_INFO(L"Lobby_Dao", 153, 101, RGB(0, 255, 0)), E_UI_LEVEL_2), vecClient[i]->GetID());
			break;
		case E_CHARACTER_TYPE_BAZZI:
			AddUI(new CUI(114 + 190 * i , 210 - 22, IMAGE_INFO(L"Lobby_Bazzi", 153, 101, RGB(0, 255, 0)), E_UI_LEVEL_2), vecClient[i]->GetID());
			break;
		}
		if (vecClient[i]->GetClientInfo()->IsRoomMaster == true)
		{
			//반장
			AddUI(new CUI_Font(150 + 190 * i , 219, "반장", E_UI_LEVEL_3, RGB(255, 0, 0)));
		}
		else
		{
			char szKey[256];
			strcpy_s(szKey, sizeof(szKey), vecClient[i]->GetID());
			strcat_s(szKey, sizeof(szKey), "_Ready");

			if (vecClient[i]->GetClientInfo() ->IsReady)
				AddUI(new CUI_Font(150 + 190 * i, 219, "준비", E_UI_LEVEL_3, RGB(255, 255, 0), L"Arial", true), szKey);
			else
				AddUI(new CUI_Font(150 + 190 * i, 219, "준비", E_UI_LEVEL_3, RGB(255, 255, 0), L"Arial", false), szKey);
		}
		AddUI(new CUI_Font(100 + 190 * i , 280, vecClient[i]->GetID(), E_UI_LEVEL_3, RGB(255, 255, 0)));
	}
}