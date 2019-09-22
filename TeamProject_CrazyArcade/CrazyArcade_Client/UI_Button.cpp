#include "stdafx.h"
#include "UI_Button.h"
#include"CollisionMgr.h"
#include"NetMgr.h"
#include"Scene.h"
#include "Client.h"
#include "MouseMgr.h"
#include"ClientMgr.h"
#include"SceneMgr.h"
#include"GameMgr.h"
#include"SoundMgr.h"
CUI_Button::CUI_Button(int iX, int iY, IMAGE_INFO _tImageInfo, E_BUTTON_TYPE eButtonType, E_UI_LEVEL _UILevel, bool IsVisible)
	: CUI(iX, iY, _tImageInfo, _UILevel)
	, m_eButtonType(eButtonType)
{
	Initialize();

	m_rcRect.right = m_rcRect.left = iX;
	m_rcRect.top = m_rcRect.bottom = iY;

	switch (eButtonType)
	{
	case E_BUTTON_TYPE_LOGIN:
		m_rcRect.left -= 50;
		m_rcRect.right += 50;
		m_rcRect.top -= 20;
		m_rcRect.bottom += 20;
		break;
	case E_BUTTON_TYPE_EXIT:
		break;
	case E_BUTTON_TYPE_GAME_START:
		m_rcRect.left -= 80;
		m_rcRect.right += 80;
		m_rcRect.top -= 20;
		m_rcRect.bottom += 20;
		break;

	case E_BUTTON_TYPE_MAP_1:
	case E_BUTTON_TYPE_MAP_2:
	case E_BUTTON_TYPE_MAP_3:
		m_rcRect.left -= 50;
		m_rcRect.right += 50;
		m_rcRect.top -= 8;
		m_rcRect.bottom += 8;
		break;
	}
	m_IsVisible = IsVisible;
}


CUI_Button::~CUI_Button()
{
}

void CUI_Button::Initialize(void)
{
	m_hDC = GetDC(g_hWnd);
}

void CUI_Button::Update(void)
{

	CUI::Update();

	if (CCollisionMgr::GetInstance()->CollMouseToRect(m_rcRect))
	{
		switch (m_eButtonType)
		{
		case E_BUTTON_TYPE_LOGIN:
		{
			m_tFrame.m_iCurFrame = 1;

			//서버 연결 
			if (GetOnceLbtnUp())
			{
				CSoundMgr::GetInstance()->PlayEnviroment(L"SFX_Button_On.wav", g_fVolume);

				TCHAR IPtmep[42];
				char IPtemp2[82];
				GetWindowText(g_hEdit2, IPtmep, 42);
				WideCharToMultiByte(CP_ACP, 0, IPtmep, 42, IPtemp2, 82, NULL, NULL);


				TCHAR Nametmep[42];
				char Nametemp2[82];
				GetWindowText(g_hEdit, Nametmep, 42);
				WideCharToMultiByte(CP_ACP, 0, Nametmep, 42, Nametemp2, 82, NULL, NULL);


				CNetMgr::GetInstance()->InitNetWorkConnect(IPtemp2, Nametemp2);
			}
		}
		break;
		case E_BUTTON_TYPE_GAME_START:
		{
			if (GetOnceLBtnDown())
			{
				CSoundMgr::GetInstance()->GetInstance()->PlayEnviroment(L"SFX_Button_On.wav", g_fVolume);


				//WaitForSingleObject(g_hMutex, INFINITE);
				CClient* pMe = CClientMgr::GetInstance()->GetMe();


				if (pMe == NULL)break;

				if (pMe->GetClientInfo()->IsRoomMaster == true)
				{
					// 모두 준비됬는지 확인하자
					// 게임 스타트 서버로 보내자 
					//서버가 판다
					CNetMgr::GetInstance()->OnRequestGameStart();
				}
				else
				{
					pMe->GetClientInfo()->IsReady = !pMe->GetClientInfo()->IsReady;
					//보내자 준비 상태 전송

					char szKey[256];
					strcpy_s(szKey, sizeof(szKey), pMe->GetID());
					strcat_s(szKey, sizeof(szKey), "_Ready");

					CScene* pScene = CSceneMgr::GetInstance()->GetScene();
					CUI* pUI = pScene->FindUI(szKey);
					SAFE_FUNC(pUI, SetVisible(pMe->GetClientInfo()->IsReady));


					CNetMgr::GetInstance()->OnRequestPushReady(pMe->GetID(), pMe->GetClientInfo()->IsReady);

				}
				//ReleaseMutex(g_hMutex);

			}
			m_tFrame.m_iCurFrame = 0;
		}
		break;
		case E_BUTTON_TYPE_MAP_1:
		case E_BUTTON_TYPE_MAP_3:
		case E_BUTTON_TYPE_MAP_2:
		{

			if (GetOnceLBtnDown())
			{
				CSoundMgr::GetInstance()->GetInstance()->PlayEnviroment(L"SFX_Empty_Off.wav", g_fVolume);

				//WaitForSingleObject(g_hMutex, INFINITE);
				CClient* pMe = CClientMgr::GetInstance()->GetMe();
				//ReleaseMutex(g_hMutex);

				if (pMe == NULL || pMe->GetClientInfo()->IsRoomMaster == false)break;


				m_IsVisible = true;

				//서버에게 알리고 나만빼고 변경
				CScene* pScene = CSceneMgr::GetInstance()->GetScene();
				if (pScene)
				{
					if (m_eButtonType == E_BUTTON_TYPE_MAP_1)
					{
						if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_1)break;

						CGameMgr::GetInstance()->SetMapType(E_MAP_TYPE_1);
						CUI* pUI = pScene->FindUI("Lobby_Map");
						SAFE_FUNC(pUI, ChangeImage(L"Lobby_Map_0"));
						SAFE_FUNC(pUI, SetVisible(true));

						pUI = pScene->FindUI("Lobby_Sel_1");
						SAFE_FUNC(pUI, SetVisible(false));

						pUI = pScene->FindUI("Lobby_Sel_2");
						SAFE_FUNC(pUI, SetVisible(false));


						CNetMgr::GetInstance()->OnRequestSelectMap(pMe->GetID(), E_MAP_TYPE_1);
					}
					else if (m_eButtonType == E_BUTTON_TYPE_MAP_2)
					{
						if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_2)break;

						CGameMgr::GetInstance()->SetMapType(E_MAP_TYPE_2);
						CUI* pUI = pScene->FindUI("Lobby_Map");
						SAFE_FUNC(pUI, ChangeImage(L"Lobby_Map_1"));
						SAFE_FUNC(pUI, SetVisible(true));

						pUI = pScene->FindUI("Lobby_Sel_0");
						SAFE_FUNC(pUI, SetVisible(false));

						pUI = pScene->FindUI("Lobby_Sel_2");
						SAFE_FUNC(pUI, SetVisible(false));

						CNetMgr::GetInstance()->OnRequestSelectMap(pMe->GetID(), E_MAP_TYPE_2);

					}
					else if (m_eButtonType == E_BUTTON_TYPE_MAP_3)
					{
						if (CGameMgr::GetInstance()->GetMapType() == E_MAP_TYPE_3)break;

						CGameMgr::GetInstance()->SetMapType(E_MAP_TYPE_3);
						CUI* pUI = pScene->FindUI("Lobby_Map");
						SAFE_FUNC(pUI, ChangeImage(L"Lobby_Map_2"));
						SAFE_FUNC(pUI, SetVisible(true));

						pUI = pScene->FindUI("Lobby_Sel_0");
						SAFE_FUNC(pUI, SetVisible(false));

						pUI = pScene->FindUI("Lobby_Sel_1");
						SAFE_FUNC(pUI, SetVisible(false));

						CNetMgr::GetInstance()->OnRequestSelectMap(pMe->GetID(), E_MAP_TYPE_3);
					}
				}
			}
		}
		break;
		default:
			break;
		}
	}
	else
	{
		switch (m_eButtonType)
		{
		case E_BUTTON_TYPE_LOGIN:
		{
			m_tFrame.m_iCurFrame = 0;
			//서버 연결 

		}
		break;
		case E_BUTTON_TYPE_GAME_START:
		{
			m_tFrame.m_iCurFrame = 1;
			break;
		}
		default:
			break;
		}
	}
}

void CUI_Button::Render(void)
{
	CUI::Render();
}
