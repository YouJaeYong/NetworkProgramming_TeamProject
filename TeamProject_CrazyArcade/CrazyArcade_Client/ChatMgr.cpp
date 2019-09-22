#include "stdafx.h"
#include "ChatMgr.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
//#include "SoundMgr.h"
IMPLEMENT_SINGLETON(CChatMgr)

CChatMgr::CChatMgr()
{
	m_fDeleteTime = 3.f;
	m_fCurTime = 0.f;
}


CChatMgr::~CChatMgr()
{
	ChatClear();
}

void CChatMgr::AddChat(const char * szChat)
{
	if (m_listChat.size() > MAX_CHATLINE)
	{
		delete m_listChat.front();
		m_listChat.pop_front();
	}


	//CSoundMgr::GetInstance()->PlayEnviroment(L"SFX_Button_Off.wav", g_fVolume);

	TCHAR strText[128];

	MultiByteToWideChar(CP_ACP, 0, szChat, 128, strText, 128);

	TMSG* pMsg = new TMSG;
	lstrcpy(pMsg->szMsg, strText);
	m_listChat.push_back(pMsg);
}

void CChatMgr::ChatClear(void)
{
	list<TMSG*>::iterator iter = m_listChat.begin();
	list<TMSG*>::iterator iter_end = m_listChat.end();

	for (iter; iter != iter_end; ++iter)
		delete *iter;
	m_listChat.clear();
}

void CChatMgr::RenderChatting(int iX, int iY, UINT iRGB)
{
	//WaitForSingleObject(g_hMutex, INFINITE);

	if (m_listChat.empty() == false && CSceneMgr::GetInstance()->GetSceneType() == E_SCENE_TYPE_TEST)
	{
		m_fCurTime += GETTIME;

		//시간 지남에따라 채팅 지워진다.
		if (m_fCurTime > m_fDeleteTime && m_listChat.empty() == false)
		{
			m_fCurTime = 0;

			delete m_listChat.front();//삭제
			m_listChat.pop_front();
		}

	}
	list<TMSG*>::reverse_iterator riter = m_listChat.rbegin();
	list<TMSG*>::reverse_iterator irter_end = m_listChat.rend();
		
	int iCurLine = 0;
	SetTextColor(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"), iRGB);

	for (riter; riter != irter_end; ++riter, ++iCurLine)
	{
		TextOut(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"), iX, iY - iCurLine * 20, (*riter)->szMsg, lstrlen((*riter)->szMsg));
	}

	SetBkMode(CBitmapMgr::GetInstance()->GetMemDC(L"BackBuffer"), TRANSPARENT);

	//ReleaseMutex(g_hMutex);

}
