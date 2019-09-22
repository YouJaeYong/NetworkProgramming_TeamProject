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

	//����
	void OnRequestChangeCharacter(const char* szID, char Character);
	void OnRequestPushReady(const char* szID, bool IsReady);
	void OnRequestSelectMap(const char* szID, E_MAP_TYPE map);

	void OnRequestGameOver(int iGameState);
	//void OnRequestChangeScene(E_SCENE_TYPE eSceneType);

	void OnSucessReady();//-->Ÿ�ϰ� ��� ���� �Ϸ� �� ȣ���� �Լ�// ��������
	//����
	void OnRequestCreateObject(CREATE_OBJECT_MSG createMsg);
	void OnRequestBubbleKick(BUBBLE_KICK_MSG bubbleKickMsg);

	//���
	void OnRequestChatting(char chat[CHAT_LEN]);//��� ä��
private:	// Notify �Լ�
	static void OnNotifyLogin(LPVOID sock);
	static void OnNotifyNewClient(LPVOID sock);
	static void OnNotifyNotifyCharInfo(LPVOID sock);
	static void OnNotifyTime(LPVOID sock);
	static void OnNotifyLoadTile(LPVOID sock);
	static void OnNotifyLoadBlock(LPVOID sock);
	//���
	static void OnNotifyChat(LPVOID sock);//ä�� ���
	//����
	static void OnNotifyPushReady(LPVOID sock);
	static void  OnNotifySelMap(LPVOID sock);
	static void OnNotifyChangeCharacter(LPVOID sock);
	static void OnNotifyGameStart(LPVOID sock);
	static void OnNotifyGameOver(LPVOID sock);
	//����-����ó��
	static void OnOverlapLogin(LPVOID sock);
	static void OnFailChoiceMap(LPVOID sock);
	static void OnFailReady(LPVOID sock);

	//����
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

