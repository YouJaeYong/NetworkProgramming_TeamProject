#include "stdafx.h"
#include "GameMgr.h"
#include "KeyMgr.h"
#include "MouseMgr.h"
#include "SceneMgr.h"
#include "Scene_Test.h"
#include "ClientMgr.h"
#include "Client.h"
#include "Character.h"

//현우
#include "NetMgr.h"
#include"UI.h"
#include"UI_Timer.h"
#include"SoundMgr.h"
IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
	
	NewGame();
}


CGameMgr::~CGameMgr()
{
}

void CGameMgr::Update(void)
{
	CalcTime();
	CSceneMgr::GetInstance()->ChangeScene();
	Controller();
	GameStart();
}

void CGameMgr::SetGameStart(bool _IsStart)
{
	printf("게임이 시작되었습니다.\n");
	m_IsGameStart = _IsStart;
	m_eGameState = E_GAME_STATE_RPOGRESS;//진행
}

void CGameMgr::SetGameState(E_GAME_STATE _eState)
{
	m_eGameState = _eState;

	CScene* pScene = CSceneMgr::GetInstance()->GetScene();

	CClient* pMe = CClientMgr::GetInstance()->GetMe();

	FUNC pFunc;
	if (pScene)
	{
		switch (m_eGameState)
		{
		case E_GAME_STATE_RPOGRESS:
			break;
		case E_GAME_STATE_BLUE_WIN:
		{
			if (pMe)
			{
				if (pMe->GetClientInfo()->cTeam== E_TEAM_TYPE_BLUE)
				{
					CUI_Timer* pUI = new CUI_Timer(WINCX / 2 - 50, WINCY / 2 - 50, 0, 2.5f, IMAGE_INFO(L"InGame_Image_Word", 405, 62, RGB(255, 0, 255)), E_UI_LEVEL_3);
					pUI->SetFrame(FRAME(0, 2, 0, 0, 0));


					CSoundMgr::GetInstance()->PlayVoice(L"SFX_Word_Win.wav", g_fVolume);
					CSoundMgr::GetInstance()->StopBGM();

					//pFunc.PFUNC = GameOverMsg;
					//pUI->SetCallBack(pFunc);
					pScene->AddUI(pUI);
				}
				else
				{
					CUI_Timer* pUI = new CUI_Timer(WINCX / 2 - 50, WINCY / 2 - 50, 0, 2.5f, IMAGE_INFO(L"InGame_Image_Word", 405, 62, RGB(255, 0, 255)), E_UI_LEVEL_3);
					pUI->SetFrame(FRAME(0, 5, 0, 0, 0));
					//pFunc.PFUNC = GameOverMsg;
					//pUI->SetCallBack(pFunc);
					pScene->AddUI(pUI);
					CSoundMgr::GetInstance()->StopBGM();
					CSoundMgr::GetInstance()->PlayVoice(L"SFX_Word_Lose.wav", g_fVolume);

				}
			}
		}
		break;
		case E_GAME_STATE_RED_WIN:
		{if (pMe)
		{
			if (pMe->GetClientInfo()->cTeam == E_TEAM_TYPE_RED)
			{
				CUI_Timer* pUI = new CUI_Timer(WINCX / 2 - 50, WINCY / 2 - 50, 0, 2.5f, IMAGE_INFO(L"InGame_Image_Word", 405, 62, RGB(255, 0, 255)), E_UI_LEVEL_3);
				pUI->SetFrame(FRAME(0, 2, 0, 0, 0));
				//pFunc.PFUNC = GameOverMsg;
				//pUI->SetCallBack(pFunc);
				pScene->AddUI(pUI);
				CSoundMgr::GetInstance()->StopBGM();
				CSoundMgr::GetInstance()->PlayVoice(L"SFX_Word_Win.wav", g_fVolume);

			}
			else
			{
				CUI_Timer* pUI = new CUI_Timer(WINCX / 2 - 50, WINCY / 2 - 50, 0, 2.5f, IMAGE_INFO(L"InGame_Image_Word", 405, 62, RGB(255, 0, 255)), E_UI_LEVEL_3);
				pUI->SetFrame(FRAME(0, 5, 0, 0, 0));
				//pFunc.PFUNC = GameOverMsg;
				//pUI->SetCallBack(pFunc);
				pScene->AddUI(pUI);
					CSoundMgr::GetInstance()->StopBGM();
					CSoundMgr::GetInstance()->PlayVoice(L"SFX_Word_Lose.wav", g_fVolume);
		
			}
		}
		}
		break;
		case E_GAME_STATE_TIME_OUT:
			CUI_Timer* pUI = new CUI_Timer(WINCX / 2 - 50, WINCY / 2 - 50, 0, 2.5f, IMAGE_INFO(L"InGame_Image_Word", 405, 62, RGB(255, 0, 255)), E_UI_LEVEL_3);
			pUI->SetFrame(FRAME(0, 4, 0, 0, 0));
			//pFunc.PFUNC = GameOverMsg;
			//pUI->SetCallBack(pFunc);
			pScene->AddUI(pUI);
			CSoundMgr::GetInstance()->StopBGM();
			CSoundMgr::GetInstance()->PlayVoice(L"SFX_Word_Draw.wav", g_fVolume);
			break;
		}
	}

}
void CGameMgr::NewGame(void)
{
	m_eMapType = E_MAP_TYPE_END;

	m_IsCollBoxRender = false;
	m_IsGameStart = false;

	m_iCurSec = 3 * 60;//3분

	CalcTime();//시간 계산

	m_eGameState = E_GAME_STATE_END;

	/*vector<CClient*> vecClient = *CClientMgr::GetInstance()->GetClient();

	for (size_t i = 0; i < vecClient.size(); ++i)
	{
		vecClient[i]->GetClientInfo()->m_IsReady = false;
		vecClient[i]->SetCharacter(NULL);
	}*/
}
bool CGameMgr::DirKeyInput(void)
{
	if (GetStayKeyDown(VK_LEFT) ||
		GetStayKeyDown(VK_RIGHT) ||
		GetStayKeyDown(VK_UP) ||
		GetStayKeyDown(VK_DOWN)
		)
	{
		return true;
	}
	return false;
}

void CGameMgr::Controller(void)
{
	if (GetOnceKeyDown('0'))
	{
		m_IsCollBoxRender = !m_IsCollBoxRender;//충돌 박스 끄기
	}
	if (GetOnceKeyDown('9'))
	{
		POINT pt = CMouseMgr::GetCurPos();
		printf("MousePos  X : %d  Y : %d \n\n", pt.x, pt.y);
	}
	if (GetOnceKeyDown('8'))
	{
		/*CClient* pClient = CClientMgr::GetInstance()->GetMe();
		if (pClient)
		{
			CCharacter* pMe = pClient->GetCharacter();
			if (pMe)
			{
				printf("ID : %s	위치 X : %f  Y : %f \n\n", pMe->GetPlayerID(), pMe->GetInfo()->vPos.x, pMe->GetInfo()->vPos.y);
			}
		}*/
	}
	if (CSceneMgr::GetInstance()->GetSceneType() == E_SCENE_TYPE_LOBBY)
	{
		LobbyControll();
	}
	else if (CSceneMgr::GetInstance()->GetSceneType() == E_SCENE_TYPE_INGAME
		|| CSceneMgr::GetInstance()->GetSceneType() == E_SCENE_TYPE_TEST)
	{
		IngameControll();
	}


}

void CGameMgr::GameStart(void)
{

}
//현우
void CGameMgr::LobbyControll(void)
{
	CClient* pMe = CClientMgr::GetInstance()->GetMe();
	CScene* pScene = CSceneMgr::GetInstance()->GetScene();
	CUI* pUI = pScene->FindUI(pMe->GetID());

	if (GetOnceKeyDown(VK_RETURN))//로비 채팅 재용
	{
		//CSoundMgr::GetInstance()->PlayEnviroment(L"SFX_Empty_Off.wav", g_fVolume);

		//WaitForSingleObject(g_hMutex, INFINITE);
		//ReleaseMutex(g_hMutex);

		if (pMe)
		{

			//채팅 리스트
			TCHAR tcChatString[42];
			char strChatString[82];
			char fullChatValue[CHAT_LEN]; // 'ID : 채팅내용' 의 양식으로 변환해서 서버에 전달하기위한 버퍼
			GetWindowText(g_hEdit, tcChatString, 42);
			WideCharToMultiByte(CP_ACP, 0, tcChatString, 42, strChatString, 82, NULL, NULL);
			SetWindowText(g_hEdit, L"");


			if (strcmp(strChatString, "") == 0)
			{
				return;
			}

			strcpy_s(fullChatValue, sizeof(fullChatValue), pMe->GetID());
			strcat_s(fullChatValue, sizeof(fullChatValue), "  :  ");
			strcat_s(fullChatValue, sizeof(fullChatValue), strChatString);

			//서버로 전달
			CNetMgr::GetInstance()->OnRequestChatting(fullChatValue);
		}
	}
	if (pMe && pUI && pMe->GetClientInfo()->IsReady == false)
	{
		if (GetOnceKeyDown(VK_LEFT) || GetOnceKeyDown(VK_RIGHT))
		{

			
			//	WaitForSingleObject(g_hMutex, INFINITE);
			if (pMe->GetClientInfo()->cCharacter == 1) {
				pMe->GetClientInfo()->cCharacter = 0;
				pMe->GetClientInfo()->cTeam = E_TEAM_TYPE_BLUE;
			}
			else if (pMe->GetClientInfo()->cCharacter == 0) {
				pMe->GetClientInfo()->cCharacter = 1;
				pMe->GetClientInfo()->cTeam = E_TEAM_TYPE_RED;
			}

				CSoundMgr::GetInstance()->GetInstance()->PlayEnviroment(L"SFX_Empty_Off.wav", g_fVolume);

			//서버로 전달
			CNetMgr::GetInstance()->OnRequestChangeCharacter(pMe->GetID(), pMe->GetClientInfo()->cCharacter);

			switch (pMe->GetClientInfo()->cCharacter)
			{
			case E_CHARACTER_TYPE_DAO:
				SAFE_FUNC(pUI, ChangeImage(L"Lobby_Dao"));
				pMe->GetClientInfo()->cTeam = E_TEAM_TYPE_BLUE;
				break;
			case E_CHARACTER_TYPE_BAZZI:
				SAFE_FUNC(pUI, ChangeImage(L"Lobby_Bazzi"));
				pMe->GetClientInfo()->cTeam = E_TEAM_TYPE_RED;
				break;
			default:
				break;

			}
			//이미지 변경 후 메세지 전달
		}

	}
}
void CGameMgr::IngameControll(void)
{
	CScene_Test* pScene = dynamic_cast<CScene_Test*>(CSceneMgr::GetInstance()->GetScene());
	CClient* pMe = CClientMgr::GetInstance()->GetMe();

	if (CSceneMgr::GetInstance()->GetSceneType() == E_SCENE_TYPE_TEST)
		CharacterControll();


	if (m_eGameState != E_GAME_STATE_RPOGRESS)
		return;//게임 끝나면 컨트롤 끄자

	if (GetOnceKeyDown(VK_RETURN))//jy
	{
		pScene->SetChatting();

		TCHAR tcChatString[42];
		char strChatString[82];
		GetWindowText(g_hEdit, tcChatString, 42);
		WideCharToMultiByte(CP_ACP, 0, tcChatString, 42, strChatString, 82, NULL, NULL);
		SetWindowText(g_hEdit, L"");
		char fullChatValue[CHAT_LEN];

		if (strcmp(strChatString, "") == 0)
		{
			return;
		}



		strcpy_s(fullChatValue, sizeof(fullChatValue), pMe->GetID());
		strcat_s(fullChatValue, sizeof(fullChatValue), "  :  ");
		strcat_s(fullChatValue, sizeof(fullChatValue), strChatString);

		//서버로 전달
		CNetMgr::GetInstance()->OnRequestChatting(fullChatValue);
	}
	if (pScene->IsChatting() == false)//채팅 중 아니면 
	{
		CharacterControll();
	}
}
void CGameMgr::CharacterControll(void)
{
	CScene* pScene = CSceneMgr::GetInstance()->GetScene();
	CUI* pUI = pScene->FindUI("");
	CClient* pMe = CClientMgr::GetInstance()->GetMe();
	if (pMe == NULL)
		return;

	CCharacter* pCharacter = pMe->GetCharacter();

	if (pCharacter == NULL)
		return;
	if (key_start == true)
	{
		cout << "밖 "<<m_eGameState << endl;
		if (m_eGameState > 0)
			{
			cout << m_eGameState << endl;
				CSceneMgr::GetInstance()->SetSceneType(E_SCENE_TYPE_LOBBY);
			}
	}
	if (pCharacter->GetState() != E_CHARACTER_STATE_DEATH
		&& pCharacter->GetState() != E_CHARACTER_STATE_REVIVE)
	{
		char cKey = 0;
	
		if (key_start == true) {
			

			if (pCharacter && pCharacter->IsControlPossible()
				&& pCharacter->IsCollBanana() == false)
			{
				//이동 가능 
				if (GetStayKeyUp(VK_LEFT) &&
					GetStayKeyUp(VK_RIGHT) &&
					GetStayKeyUp(VK_UP) &&
					GetStayKeyUp(VK_DOWN)
					)
				{
					pCharacter->SetMoveState(E_MOVE_STAND);
				}
				if (GetStayKeyDown(VK_LEFT))
				{
					cKey |= KEY_LEFT;
					pCharacter->SetMoveState(E_MOVE_LEFT);
				}
				if (GetStayKeyDown(VK_RIGHT))
				{
					cKey |= KEY_RIGHT;
					pCharacter->SetMoveState(E_MOVE_RIGHT);
				}
				if (GetStayKeyDown(VK_UP))
				{
					cKey |= KEY_UP;
					pCharacter->SetMoveState(E_MOVE_UP);
				}
				if (GetStayKeyDown(VK_DOWN))
				{
					cKey |= KEY_DOWN;
					pCharacter->SetMoveState(E_MOVE_DOWN);
				}

				pCharacter->SetKey(cKey);


				if (GetOnceKeyDown('1'))//핀 사용
				{
					pCharacter->UsePin();
				}
				if (GetOnceKeyDown('2'))//다트 사용
				{
					pCharacter->UseDart();
				}
				if (GetOnceKeyDown('3'))//마인 사용
				{
					pCharacter->UseMine();
				}

				if (GetOnceKeyDown('4'))//바나나 사용
				{
					pCharacter->UseBanana();
				}
				if (GetOnceKeyDown(VK_SPACE))
				{
					pCharacter->CreateBubble();
				}
			}
		}
		else {
			pCharacter->SetKey(cKey);
		}
	}
}
// 시간 계산

void CGameMgr::CalcTime(void)
{
	m_iMinute = m_iCurSec / 60;
	m_iFirst_sec = (m_iCurSec % 60) / 10;
	m_iSecond_sec = (m_iCurSec % 60) % 10;
	if (m_iCurSec==0) {
		GameResult();
		m_iCurSec = 3 * 60;//3분
	}
}
bool CGameMgr::GameResult(void)
{
	CNetMgr::GetInstance()->OnRequestGameOver(E_GAME_STATE_TIME_OUT);
	//게임 결과 서버로 전달 
	return true;
	//vector<CClient*> vecClient = *CClientMgr::GetInstance()->GetClient();
	//bool IsBlueTeam = false;
	//bool IsRedTeam = false;

	//for (size_t i = 0; i < vecClient.size(); ++i)
	//{
	//	if (vecClient[i]->GetCharacter() == NULL)continue;

	//	if (vecClient[i]->GetClientInfo()->cTeam == E_TEAM_TYPE_BLUE)
	//	{
	//		IsBlueTeam = true;
	//	}
	//	else if (vecClient[i]->GetClientInfo()->cTeam == E_TEAM_TYPE_RED)
	//	{
	//		IsRedTeam = true;
	//	}
	//}

	//if (IsBlueTeam == true && IsRedTeam == false)
	//{
	//	if (m_eGameState == E_GAME_STATE_RPOGRESS)
	//		CNetMgr::GetInstance()->OnRequestGameOver(E_GAME_STATE_BLUE_WIN);
	//	//서버로 전달.
	//	return true;//블루팀 승리
	//}
	//else if (IsBlueTeam == false && IsRedTeam == true)
	//{
	//	//서버로 전달
	//	if (m_eGameState == E_GAME_STATE_RPOGRESS)
	//		CNetMgr::GetInstance()->OnRequestGameOver(E_GAME_STATE_RED_WIN);
	//	return true;//레드팀 승리
	//}
	//return false;

	//return false;
}
//void CGameMgr::GameOverMsg(void)
//{
//	CNetMgr::GetInstance()->OnRequestChangeScene(E_SCENE_TYPE_LOBBY);
//	//서버로 게임 오버 메시지 전달
//}
