#include "stdafx.h"
#include "Scene_Test.h"
#include "UI_ImageFont.h"
#include "GameMgr.h"
#include "MapMgr.h"
#include "ObjectMgr.h"
#include "BitmapMgr.h"
#include "NetMgr.h"
#include "ClientMgr.h"
#include "Client.h"
#include "UI_Font.h"
#include "Bazzi.h"
#include "Dao.h"
#include "NetMgr.h"
#include "UI_Timer.h"
#include "ChatMgr.h"
#include"SoundMgr.h"
CScene_Test::CScene_Test()
{
}


CScene_Test::~CScene_Test()
{
	Release();
}
void CScene_Test::Initialize(void)
{
	CUI_Timer* pUI = new CUI_Timer(WINCX / 2 - 50, WINCY / 2 - 50, 0, 1.5f, IMAGE_INFO(L"InGame_Image_Word", 405, 62, RGB(255, 0, 255)), E_UI_LEVEL_3);
	pUI->SetFrame(FRAME(0, 0, 0, 0, 0));
	AddUI(pUI);


	AddUI(new CUI_ImageFont(CGameMgr::GetInstance()->GetMinute(), 725, 82, IMAGE_INFO(L"InGame_Image_Num", 10, 12, RGB(0, 0, 255)), E_UI_LEVEL_3));
	AddUI(new CUI_ImageFont(CGameMgr::GetInstance()->GetFirst_sece(), 745, 82, IMAGE_INFO(L"InGame_Image_Num", 10, 12, RGB(0, 0, 255)), E_UI_LEVEL_3));
	AddUI(new CUI_ImageFont(CGameMgr::GetInstance()->GetSecond_sec(), 755, 82, IMAGE_INFO(L"InGame_Image_Num", 10, 12, RGB(0, 0, 255)), E_UI_LEVEL_3));

	//테스트용
	//CGameMgr::GetInstance()->SetMapType(E_MAP_TYPE_1);
	//	CMapMgr::GetInstance()->LoadMap();//맵 로딩

	// 나 생성


	//char szID[10] = { 0 , };
	//_itoa_s(rand() % 100, szID, 10);

	////서버 연결
	//CNetMgr::GetInstance()->InitNetWorkConnect("127.0.0.1", szID);
	//채팅
	g_hEdit = CreateWindow(L"edit", L"", //JY
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER,
		230, 530, 200, 22,
		g_hWnd, (HMENU)100, NULL, NULL);
	SendMessage(g_hEdit, EM_LIMITTEXT, (WPARAM)40, 0);


	//CNetMgr::GetInstance()->OnRequestGameStart();
	//캐릭터 생성
	CreateCharacter();
		InitClient();//Client UI 생성 테스트는 매니저에서 함
	//InitClient();


	UISortByLevel();
	if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_1)
	{
		CSoundMgr::GetInstance()->PlayBGM(L"BGM_Map_0.wav", true, g_fVolume);
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_2)
	{
		CSoundMgr::GetInstance()->PlayBGM(L"BGM_Map_1.wav", true, g_fVolume);
	}
	else if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_3)
	{
		CSoundMgr::GetInstance()->PlayBGM(L"BGM_Map_2_0.wav", true, g_fVolume);
	}

	CSoundMgr::GetInstance()->PlayVoice(L"SFX_Word_Start.wav", g_fVolume);

}
void CScene_Test::Update()
{
	UIUpdate();
	CObjectMgr::GetInstance()->Update();
}
void CScene_Test::Render(void)
{
	BitBlt(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"),	//복사될 대상 DC
		0,			//복사될 좌표 위치X
		0,			//복사될 좌표 위치Y
		WINCX,		//이미지 크기 X
		WINCY,		//이미지 크기 Y
		CBitmapMgr::GetInstance()->GetMemDC(L"InGame_Bg"),	//복사할 DC
		0,	//이미지 어디부터 출력할 것인가?
		0,			//이미지 어디부터 출력할 것인가?
		SRCCOPY);
	CMapMgr::GetInstance()->Render();
	CObjectMgr::GetInstance()->Render();
	UIRender();
	if (CChatMgr::GetInstance()->GetChatList().empty() == false)
	{
		BLENDFUNCTION bf;

		bf.AlphaFormat = 0;
		bf.BlendFlags = 0;
		bf.BlendOp = 0;
		bf.SourceConstantAlpha = 100;

		AlphaBlend(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"), 50, 330, 150, 185, CBitmapMgr::GetInstance()->GetMemDC(L"Alpha"), 0, 0, 100, 100, bf);
		CChatMgr::GetInstance()->RenderChatting(50, 500, RGB(255, 255, 0));

	}

	
	if (m_IsChatting)//JY
	{
		RECT rt = { 230, 530, 230 + 200 ,530 + 22 };
		InvalidateRect(g_hWnd, &rt, false);
	}
}
void CScene_Test::SetChatting(void)//JY
{
	m_IsChatting = !m_IsChatting;

	if (m_IsChatting)
	{
		ShowWindow(g_hEdit, SW_SHOW);
		SetFocus(g_hEdit);
	}
	else
	{
		ShowWindow(g_hEdit, SW_HIDE);
	}
}
CScene_Test* CScene_Test::Create(void)
{
	CScene_Test* pTest = new CScene_Test;
	pTest->Initialize();
	return pTest;
}

void CScene_Test::InitClient(void)
{
	vector<CClient*>*	vecClient = CClientMgr::GetInstance()->GetClient();
	for (size_t i = 0; i < vecClient->size(); ++i)
	{
		switch ((*vecClient)[i]->GetClientInfo()->cCharacter)
		{
		case E_CHARACTER_TYPE_DAO:
			AddUI(new CUI(682, 119 + 42 * i, IMAGE_INFO(L"InGame_Dao", 42, 31, RGB(2, 255, 2)), E_UI_LEVEL_2), (*vecClient)[i]->GetID());
			break;
		case E_CHARACTER_TYPE_BAZZI:
			AddUI(new CUI(682, 119 + 42 * i, IMAGE_INFO(L"InGame_Bazzi", 42, 31, RGB(2, 255, 2)), E_UI_LEVEL_2), (*vecClient)[i]->GetID());
			break;
		}
		AddUI(new CUI_Font(739, 103 + 43 * i, (*vecClient)[i]->GetID(), E_UI_LEVEL_3, RGB(255, 255, 255)));

	}

}

void CScene_Test::Release()
{
	CMapMgr::GetInstance()->MapDestory();
	CObjectMgr::GetInstance()->Clear();
	vector<CClient*>* vecClient = CClientMgr::GetInstance()->GetClient();
	for (int i = 0; i < (*vecClient).size(); ++i) {
		(*vecClient)[i]->GameEnd(false);
	}
	DestroyWindow(g_hEdit);
}

void CScene_Test::CreateCharacter(void)
{
	vector<CClient*>* vecClient = CClientMgr::GetInstance()->GetClient();

	for (size_t i = 0; i < vecClient->size(); ++i)
	{
		if ((*vecClient)[i]->IsGameEnd()) {
			continue;
		}
		(*vecClient)[i]->GameEnd(true);
		CCharacter* pCharacter;

		switch ((*vecClient)[i]->GetClientInfo()->cCharacter)
		{
		case E_CHARACTER_TYPE_BAZZI:
		{
			pCharacter = CBazzi::Create((*vecClient)[i]->GetID(),(int)(*vecClient)[i]->GetClientInfo()->cStartPos);
			(*vecClient)[i]->SetCharacter(pCharacter);
			CObjectMgr::GetInstance()->AddObject(pCharacter);

			CHAR_INFO_MSG	info;
			strcpy_s(info.szID, pCharacter->GetPlayerID());
			info.vPos = pCharacter->GetInfo()->vPos;
			info.vDir = pCharacter->GetInfo()->m_vDir;
			info.iState = pCharacter->GetState();
			info.iMoveState = pCharacter->GetMoveState();
			CNetMgr::GetInstance()->OnRequestCharacterInfo(info);
		}
		break;
		case E_CHARACTER_TYPE_DAO:
		{
			pCharacter = CDao::Create((*vecClient)[i]->GetID(), (int)(*vecClient)[i]->GetClientInfo()->cStartPos);
			(*vecClient)[i]->SetCharacter(pCharacter);
			CObjectMgr::GetInstance()->AddObject(pCharacter);

			CHAR_INFO_MSG	info;
			strcpy_s(info.szID, pCharacter->GetPlayerID());
			info.vPos = pCharacter->GetInfo()->vPos;
			info.vDir = pCharacter->GetInfo()->m_vDir;
			info.iState = pCharacter->GetState();
			info.iMoveState = pCharacter->GetMoveState();
			CNetMgr::GetInstance()->OnRequestCharacterInfo(info);

		}
		break;
		}
	}
}
