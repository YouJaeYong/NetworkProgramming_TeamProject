#include "stdafx.h"
#include "ClientMgr.h"
#include "Client.h"
#include "Scene_Test.h"
#include "SceneMgr.h"
#include"Scene_Lobby.h"
#include"SoundMgr.h"
IMPLEMENT_SINGLETON(CClientMgr)
CClientMgr::CClientMgr()
{
}


CClientMgr::~CClientMgr()
{
}
void CClientMgr::AddClient(CClient* pClient)
{
	CSoundMgr::GetInstance()->PlayEnviroment(L"SFX_Empty_On.wav", g_fVolume);

	CScene_Lobby* pScene_Lobby = dynamic_cast<CScene_Lobby*>(CSceneMgr::GetInstance()->GetScene());
	if (pScene_Lobby)
	{
		pScene_Lobby->AddClient(pClient, m_vecClient.size());
	}
	m_vecClient.push_back(pClient);//순서 중요 인덱스 넘겨서

	CScene_Test* pScene_Test = dynamic_cast<CScene_Test*>(CSceneMgr::GetInstance()->GetScene());
	if (pScene_Test)
	{
		pScene_Test->CreateCharacter();
		pScene_Test->InitClient();//UI 이미지 추가
	}



}

CClient * CClientMgr::FindClient(const char * szID)
{
	for (size_t i = 0; i < m_vecClient.size(); ++i)
	{
		if (strcmp(m_vecClient[i]->GetID(), szID) == 0)
		{
			return m_vecClient[i];
		}
	}
	return NULL;
}
