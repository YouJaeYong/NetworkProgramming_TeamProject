#pragma once
#include "Include.h"

#define MAX_CHATLINE		8
typedef struct tagTMSG
{
	TCHAR		szMsg[128];
}TMSG;

class CChatMgr
{
	DECLARE_SINGLETON(CChatMgr)
private:
	CChatMgr();
	~CChatMgr();
public:
	void AddChat(const char* szChat);
	void ChatClear(void);
	void RenderChatting(int iX, int iY, UINT iRGB = RGB(255, 255, 255));
	list<TMSG*> GetChatList(void) { return m_listChat; }
private:
	list<TMSG*>	m_listChat;
	float m_fDeleteTime;
	float m_fCurTime;
};

