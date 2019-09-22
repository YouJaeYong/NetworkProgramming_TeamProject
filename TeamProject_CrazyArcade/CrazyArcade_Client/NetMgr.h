#pragma once
#include "Include.h"
#include "Protocol.h"


typedef struct NetFunc_Info
{
	void(*func)(void*);
}NET_FUNC;

class CNetMgr
{
	DECLARE_SINGLETON(CNetMgr)
public:
	CNetMgr();
	~CNetMgr();
public:
	int InitNetWorkConnect(const char* szIP, const char* szID);
private:
	void FuncInitialize();
	static DWORD WINAPI RecvThread(LPVOID sock);

public://Request 
	void OnRequestLogin(const char* szID);
	void OnRequestCharacterInfo(const CHAR_INFO_MSG& Msg);
	void OnRequestGameStart();

	//현우
	void OnRequestChangeCharacter(const char* szID, char Character);
	void OnRequestPushReady(const char* szID, bool IsReady);
	void OnRequestSelectMap(const char* szID, E_MAP_TYPE map);

	void OnRequestGameOver(int iGameState);
	//void OnRequestChangeScene(E_SCENE_TYPE eSceneType);

	void OnSucessReady();//-->타일과 블록 전송 완료 후 호출함 함수// 만들어야함
	//우혁
	void OnRequestCreateObject(CREATE_OBJECT_MSG createMsg);
	void OnRequestBubbleKick(BUBBLE_KICK_MSG bubbleKickMsg);

	//재용
	void OnRequestChatting(char chat[CHAT_LEN]);//재용 채팅
private:	// Notify 함수
	static void OnNotifyLogin(LPVOID sock);
	static void OnNotifyNewClient(LPVOID sock);
	static void OnNotifyNotifyCharInfo(LPVOID sock);
	static void OnNotifyTime(LPVOID sock);
	static void OnNotifyLoadTile(LPVOID sock);
	static void OnNotifyLoadBlock(LPVOID sock);
	//재용
	static void OnNotifyChat(LPVOID sock);//채팅 재용
	//현우
	static void OnNotifyPushReady(LPVOID sock);
	static void  OnNotifySelMap(LPVOID sock);
	static void OnNotifyChangeCharacter(LPVOID sock);
	static void OnNotifyGameStart(LPVOID sock);
	static void OnNotifyGameOver(LPVOID sock);
	//현우-예외처리
	static void OnOverlapLogin(LPVOID sock);
	static void OnFailChoiceMap(LPVOID sock);
	static void OnFailReady(LPVOID sock);

	//우혁
	static void OnNotifyCreateObject(LPVOID sock);
	static void OnNotifyRemoveObject(LPVOID sock);
	static void OnNotifyBubbleKick(LPVOID sock);
private:
	WSADATA			m_wsaData;
	SOCKET			m_hSocket;
	SOCKADDR_IN		m_ServAddr;
	HANDLE			m_hThread;
	bool			m_IsSuccessConnect;
	static vector<NET_FUNC>		g_vecFunc;


};

