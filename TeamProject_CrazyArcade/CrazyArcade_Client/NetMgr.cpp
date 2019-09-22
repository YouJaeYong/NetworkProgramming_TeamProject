#include "stdafx.h"
#include "NetMgr.h"
#include "NetInclude.h"
#include "GameMgr.h"
#include "Client.h"
#include "ClientMgr.h"
#include "NetFunc.h"
#include "Character.h"
#include "Tile.h"
#include "Block.h"
#include "MapMgr.h"
//재용
#include "ChatMgr.h"//채팅

//우혁
#include "Object.h"
#include "Bubble.h"
#include "ObjectMgr.h"
//현우
#include "SceneMgr.h"
#include"UI.h"
#include"Scene.h"
#include"Dart.h"
IMPLEMENT_SINGLETON(CNetMgr)
vector<NET_FUNC> CNetMgr::g_vecFunc;

CNetMgr::CNetMgr()
{
	WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	m_IsSuccessConnect = false;
}


CNetMgr::~CNetMgr()
{
	closesocket(m_hSocket);
	CloseHandle(m_hThread);
	WSACleanup();
}

int CNetMgr::InitNetWorkConnect(const char* szIP, const char* szID)
{
	if (m_IsSuccessConnect == false)
	{
		m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_hSocket == INVALID_SOCKET)
			err_quit("socket()");
		ZeroMemory(&m_ServAddr, sizeof(m_ServAddr));
		m_ServAddr.sin_family = AF_INET;
		m_ServAddr.sin_addr.s_addr = inet_addr(szIP);
		m_ServAddr.sin_port = htons(SERVER_PORT);

		if (connect(m_hSocket, (SOCKADDR*)&m_ServAddr, sizeof(m_ServAddr)) == SOCKET_ERROR)
		{
			err_quit("connect()");
			return 1;//재접속
		}
		else
		{
			printf("Server Connect Success : %s Port %d\n", szIP, SERVER_PORT);
			FuncInitialize();//함수 셋팅
			m_hThread = (HANDLE)CreateThread(NULL, 0, RecvThread, (LPVOID)&m_hSocket, 0, NULL);
			m_IsSuccessConnect = true;

			OnRequestLogin(szID);
			m_IsSuccessConnect = false; //재용 ㅠㅠ
		}
	}
	return 0;
}

void CNetMgr::FuncInitialize()
{
	g_vecFunc.resize(E_MSG_TYPE_END);
	g_vecFunc[E_MSG_TYPE_NOTIFY_LOGIN].func = OnNotifyLogin;
	g_vecFunc[E_MSG_TYPE_NOTIFY_NEW_CLIENT].func = OnNotifyNewClient;
	g_vecFunc[E_MSG_TYPE_NOTIFY_CHARINFO].func = OnNotifyNotifyCharInfo;
	g_vecFunc[E_MSG_TYPE_NOTIFY_TIME].func = OnNotifyTime;
	g_vecFunc[E_MSG_TYPE_NOTIFY_LOAD_TILE].func = OnNotifyLoadTile;
	g_vecFunc[E_MSG_TYPE_NOTIFY_LOAD_BLOCK].func = OnNotifyLoadBlock;

	//전현우
	g_vecFunc[E_MSG_TYPE_NOTIFY_PUSH_READY].func = OnNotifyPushReady;//준비 상태 변경
	g_vecFunc[E_MSG_TYPE_NOTIFY_SEL_MAP].func = OnNotifySelMap;//맵 선택 변경
	g_vecFunc[E_MSG_TYPE_NOTIFY_CHANGE_CHARACTER].func = OnNotifyChangeCharacter;//캐릭터 변경& 팀변경
	g_vecFunc[E_MSG_TYPE_NOTIFY_GAME_OVER].func = OnNotifyGameOver;

	g_vecFunc[E_MSG_TYPE_NOTIFY_GAME_START].func = OnNotifyGameStart;//게임 시작알림
	//우혁

	g_vecFunc[E_MSG_TYPE_NOTIFY_CREATE_OBJECT].func = OnNotifyCreateObject;

	//예외처리-현우
	g_vecFunc[E_MSG_TYPE_FAIL_GAMESTART_CHOICE_MAP].func = OnFailChoiceMap;//게임 시작 실패
	g_vecFunc[E_MSG_TYPE_FAIL_GAMESTART_READY].func = OnFailReady;//게임 시작 실패
	g_vecFunc[E_MSG_TYPE_OVERLAP_LOGIN].func = OnOverlapLogin;//게임 시작 실패
	g_vecFunc[E_MSG_TYPE_NOTIFY_REMOVE_OBJECT].func = OnNotifyRemoveObject;
	g_vecFunc[E_MSG_TYPE_NOTIFY_BUBBLE_KICK].func = OnNotifyBubbleKick;

	//재용

	g_vecFunc[E_MSG_TYPE_NOTIFY_CHAT].func = OnNotifyChat; //재용 채팅
}

DWORD WINAPI CNetMgr::RecvThread(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	int			retval;
	int				iHeader;
	while (1)
	{
		//헤더 정보 받기 ex ) E_MSG_TYPE_MOVE 
		retval = recvn(hClntSock, (char*)&iHeader, sizeof(iHeader), 0);
		if (retval == 0 || retval == SOCKET_ERROR)
		{
			//연결 끊김
			printf("Disconnected Server\n");
			return 0;
		}
		else if (0 <= iHeader && iHeader < (int)g_vecFunc.size())
		{
			if (g_vecFunc[iHeader].func)
				g_vecFunc[iHeader].func(&hClntSock);
		}
	}

}

void CNetMgr::OnRequestLogin(const char* szID)
{
	int protocol = E_MSG_TYPE_REQUEST_LOGIN;
	LOGIN_MSG	LoginMsg;
	strcpy_s(LoginMsg.ID, szID);

	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&LoginMsg, sizeof(LOGIN_MSG), 0);
}

void CNetMgr::OnRequestCharacterInfo(const CHAR_INFO_MSG& Msg)
{
	int protocol = E_MSG_TYPE_REQUEST_CHARINFO;
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&Msg, sizeof(CHAR_INFO_MSG), 0);
}

void CNetMgr::OnRequestGameStart()//JWH 테스트용
{
	int protocol = E_MSG_TYPE_REQUEST_GAME_START;
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
}

//현우

void CNetMgr::OnRequestChangeCharacter(const char* szID, char Character) {
	int protocol = E_MSG_TYPE_REQUEST_CHANGE_CHARACTER;
	ChangeCharacter Character_info;
	sprintf_s(Character_info.ID, szID);
	Character_info.cCharacter = Character;
	Character_info.cTeam = Character;//원래는 팀이지만 캐릭터랑 같으므로 동일 처리
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&Character_info, sizeof(ChangeCharacter), 0);
}
void CNetMgr::OnRequestPushReady(const char* szID, bool IsReady) {
	int protocol = E_MSG_TYPE_REQUEST_PUSH_READY;
	ReadyPush Ready_User;
	sprintf_s(Ready_User.ID, szID);
	Ready_User.IsReady = IsReady;
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&Ready_User, sizeof(ReadyPush), 0);
}
void CNetMgr::OnRequestSelectMap(const char* szID, E_MAP_TYPE map) {
	int protocol = E_MSG_TYPE_REQUEST_SEL_MAP;
	char MapType = CGameMgr::GetInstance()->GetMapType();
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&MapType, sizeof(char), 0);
}

void CNetMgr::OnRequestCreateObject(CREATE_OBJECT_MSG createMsg)
{
	int protocol = E_MSG_TYPE_REQUEST_CREATE_OBJECT;

	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&createMsg, sizeof(CREATE_OBJECT_MSG), 0);

}

void CNetMgr::OnRequestBubbleKick(BUBBLE_KICK_MSG bubbleKickMsg)
{
	int protocol = E_MSG_TYPE_REQUEST_BUBBLE_KICK;

	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&bubbleKickMsg, sizeof(BUBBLE_KICK_MSG), 0);
}






void CNetMgr::OnSucessReady() {
	int protocol = E_MSG_TYPE_SUCESS_READY;

	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	vector<CClient*> vecClnt = *CClientMgr::GetInstance()->GetClient();
	for (int i = 0; i > vecClnt.size(); ++i) {
		vecClnt[i]->SetReady(false);
	}
}


void CNetMgr::OnRequestChatting(char chat[CHAT_LEN]) {//재용
	int protocol = E_MSG_TYPE_REQUEST_CHAT;
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)chat, CHAT_LEN, 0);

}

void CNetMgr::OnRequestGameOver(int iGameState)
{
	int protocol = E_MSG_TYPE_REQUEST_GAME_OVER;	
	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
	send(m_hSocket, (char*)&iGameState, sizeof(int), 0);
}
//void CNetMgr::OnRequestChangeScene(E_SCENE_TYPE eSceneType)
//{
//	int iType = (int)eSceneType;
//
//
//	int protocol = E_MSG_TYPE_REQUEST_CHANGE_SCENE;
//	send(m_hSocket, (char*)&protocol, sizeof(int), 0);
//	send(m_hSocket, (char*)iType, sizeof(int), 0);
//}


//-------------------------------------------------------------------//











void CNetMgr::OnNotifyLogin(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	NOTIFY_LOGIN_MSG	LoginMsg;
	recvn(hClntSock, (char*)&LoginMsg, sizeof(NOTIFY_LOGIN_MSG), 0);


	////ME 생성 후 ID 셋팅
	CClient* pClient = new CClient(LoginMsg.info.szID);
	pClient->GetClientInfo()->IsRoomMaster = LoginMsg.info.IsRoomMaster;
	pClient->GetClientInfo()->IsReady = LoginMsg.info.IsReady;
	pClient->GetClientInfo()->cTeam = LoginMsg.info.cTeam;
	pClient->GetClientInfo()->cCharacter = LoginMsg.info.cCharacter;
	pClient->GetClientInfo()->cStartPos = LoginMsg.info.cStartPos;
	CGameMgr::GetInstance()->SetMapType(LoginMsg.imaptype);
	CClientMgr::GetInstance()->SetMe(pClient);

	//현우 테스트
	CSceneMgr::GetInstance()->SetSceneType(E_SCENE_TYPE_LOBBY);
}

void CNetMgr::OnNotifyNewClient(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	CLIENT_INFO	ClientInfo;
	recvn(hClntSock, (char*)&ClientInfo, sizeof(CLIENT_INFO), 0);

	CClient* pMe = CClientMgr::GetInstance()->GetMe();

	if (pMe)
	{
		if (strcmp(pMe->GetID(), ClientInfo.szID) == 0)
		{
			CClientMgr::GetInstance()->AddClient(pMe);
		}
		else
		{
			CClient* pClient = new CClient(ClientInfo.szID);
			pClient->GetClientInfo()->IsRoomMaster = ClientInfo.IsRoomMaster;
			pClient->GetClientInfo()->IsReady = ClientInfo.IsReady;
			pClient->GetClientInfo()->cTeam = ClientInfo.cTeam;
			pClient->GetClientInfo()->cCharacter = ClientInfo.cCharacter;
			pClient->GetClientInfo()->cStartPos = ClientInfo.cStartPos;
			CClientMgr::GetInstance()->AddClient(pClient);
		}
	}
}

void CNetMgr::OnNotifyNotifyCharInfo(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	CHAR_INFO_MSG	charInfo;
	recvn(hClntSock, (char*)&charInfo, sizeof(CHAR_INFO_MSG), 0);
		vector<CClient*> vecClnt = *CClientMgr::GetInstance()->GetClient();
	// 위치 값 등록
	for (size_t i = 0; i < vecClnt.size(); ++i)
	{
		CClient* pClient = vecClnt[i];
		if (strcmp(charInfo.szID, pClient->GetID()) == 0)
		{
			if (pClient->GetCharacter())
			{
				//Next 위치 셋팅
				pClient->GetCharacter()->SetInfoMsg(charInfo);
				pClient->GetCharacter()->SetInitPos(true);
				/*	pClient->GetCharacter()->GetInfo()->vPos = charInfo.vPos;
					pClient->GetCharacter()->GetInfo()->m_vDir = charInfo.vDir;
					pClient->GetCharacter()->SetState((E_CHARACTER_STATE)charInfo.iState);*/

			}
		}
	}

}

void CNetMgr::OnNotifyTime(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	// 1초 지남
	CGameMgr::GetInstance()->SetTime();
}

void CNetMgr::OnNotifyLoadTile(LPVOID sock)
{
	//현우
	SOCKET	hClntSock = *((SOCKET*)sock);
	LOADTILE_MSG	LoadTilemsg;
	recvn(hClntSock, (char*)&LoadTilemsg, sizeof(LOADTILE_MSG), 0);

	int iType = LoadTilemsg.iMapType;
	for (int i = 0; i < TILE_CNTY; ++i) {
		for (int j = 0; j < TILE_CNTX; ++j) {
			CTile* pTile = new CTile(LoadTilemsg.i_AllX[i][j], LoadTilemsg.i_AllY[i][j]);
			pTile->SetFrame(LoadTilemsg.i_AllOption[i][j]);
			switch (iType)
			{
			case E_MAP_TYPE_1:
				pTile->SetImageInfo(IMAGE_INFO(L"Map_0_Tile", 40, 40, RGB(255, 255, 255)));
				break;
			case E_MAP_TYPE_2:
				pTile->SetImageInfo(IMAGE_INFO(L"Map_1_Tile", 40, 40, RGB(255, 255, 255)));
				break;
			case E_MAP_TYPE_3:
				pTile->SetImageInfo(IMAGE_INFO(L"Map_2_Tile", 40, 40, RGB(255, 255, 255)));
				break;
			default:
				pTile->SetImageInfo(IMAGE_INFO(L"Box_-1", 40, 40, RGB(255, 255, 255)));
				break;
			}
			//추가
			CMapMgr::GetInstance()->AddTile(pTile);
		}
	}
	printf("타일 완료\n");

}

void CNetMgr::OnNotifyLoadBlock(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	LOADBLOCK_MSG	blockMsg;

	recvn(hClntSock, (char*)&blockMsg, sizeof(LOADBLOCK_MSG), 0);

	CBlock* pBlock = new CBlock(blockMsg.iID, (float)blockMsg.iX, (float)blockMsg.iY, IMAGE_INFO(blockMsg.strKey, blockMsg.iWidth, blockMsg.iHeight, blockMsg.iRGB), (E_BLOCK_TYPE)blockMsg.iBlockType, blockMsg.iItemType);
	CMapMgr::GetInstance()->SetBlockCnt(blockMsg.iCnt);
	CMapMgr::GetInstance()->AddBlock(pBlock);

	if (CMapMgr::GetInstance()->BlockReady()) {

		printf("블록 완료\n");
		CNetMgr::GetInstance()->OnSucessReady();
	}
}

//현우


void CNetMgr::OnNotifyChangeCharacter(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	ChangeCharacter Character_info;
	recvn(hClntSock, (char*)&Character_info, sizeof(ChangeCharacter), 0);
	vector<CClient*> vecClnt = *CClientMgr::GetInstance()->GetClient();
	for (int i = 0; i < vecClnt.size(); ++i) {
		CClient* pClient = vecClnt[i];
		if (strcmp(pClient->GetID(), Character_info.ID) == 0) {
			CScene* pScene = CSceneMgr::GetInstance()->GetScene();
			CUI* pUI = pScene->FindUI(pClient->GetID());
			pClient->SetCharacter(Character_info.cCharacter);
			switch (pClient->GetClientInfo()->cCharacter)
			{
			case E_CHARACTER_TYPE_DAO:
				pClient->GetClientInfo()->cTeam = E_TEAM_TYPE_BLUE;
				SAFE_FUNC(pUI, ChangeImage(L"Lobby_Dao"));
				break;
			case E_CHARACTER_TYPE_BAZZI:
				pClient->GetClientInfo()->cTeam = E_TEAM_TYPE_RED;
				SAFE_FUNC(pUI, ChangeImage(L"Lobby_Bazzi"));
				break;
			default:
				break;
			}
			break;
		}
	}
}
void CNetMgr::OnNotifySelMap(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	int maptype;
	//문제 발생...자꾸 11로들어오네.. char이건 int건..
	recvn(hClntSock, (char*)&maptype, sizeof(int), 0);
	CScene* pScene = CSceneMgr::GetInstance()->GetScene();
	CGameMgr::GetInstance()->SetMapType(maptype);
	CUI* pUI = pScene->FindUI("Lobby_Map");
	SAFE_FUNC(pUI, SetVisible(true));
	switch (maptype)
	{
	case E_MAP_TYPE_1:
		SAFE_FUNC(pUI, ChangeImage(L"Lobby_Map_0"));
		pUI = pScene->FindUI("Lobby_Sel_0");
		SAFE_FUNC(pUI, SetVisible(true));
		pUI = pScene->FindUI("Lobby_Sel_1");
		SAFE_FUNC(pUI, SetVisible(false));
		pUI = pScene->FindUI("Lobby_Sel_2");
		SAFE_FUNC(pUI, SetVisible(false));
		break;
	case E_MAP_TYPE_2:
		SAFE_FUNC(pUI, ChangeImage(L"Lobby_Map_1"));
		pUI = pScene->FindUI("Lobby_Sel_0");
		SAFE_FUNC(pUI, SetVisible(false));
		pUI = pScene->FindUI("Lobby_Sel_1");
		SAFE_FUNC(pUI, SetVisible(true));
		pUI = pScene->FindUI("Lobby_Sel_2");
		SAFE_FUNC(pUI, SetVisible(false));
		break;
	case E_MAP_TYPE_3:
		SAFE_FUNC(pUI, ChangeImage(L"Lobby_Map_2"));
		pUI = pScene->FindUI("Lobby_Sel_0");
		SAFE_FUNC(pUI, SetVisible(false));
		pUI = pScene->FindUI("Lobby_Sel_1");
		SAFE_FUNC(pUI, SetVisible(false));
		pUI = pScene->FindUI("Lobby_Sel_2");
		SAFE_FUNC(pUI, SetVisible(true));
		break;
	default:
		break;
	}
}
void CNetMgr::OnNotifyPushReady(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	ReadyPush Ready_Push;
	recvn(hClntSock, (char*)&Ready_Push, sizeof(ReadyPush), 0);
	vector<CClient*> vecClnt = *CClientMgr::GetInstance()->GetClient();
	for (int i = 0; i < vecClnt.size(); ++i) {
		CClient* pClient = vecClnt[i];
		if (strcmp(pClient->GetID(), Ready_Push.ID) == 0) {
			pClient->SetReady(Ready_Push.IsReady);
			char szKey[256];
			strcpy_s(szKey, sizeof(szKey), pClient->GetID());
			strcat_s(szKey, sizeof(szKey), "_Ready");
			CScene* pScene = CSceneMgr::GetInstance()->GetScene();
			CUI* pUI = pScene->FindUI(szKey);
			SAFE_FUNC(pUI, SetVisible(Ready_Push.IsReady));
			break;
		}
	}
}
void CNetMgr::OnNotifyGameStart(LPVOID sock) {
	CSceneMgr::GetInstance()->SetSceneType(E_SCENE_TYPE_TEST);
	CGameMgr::GetInstance()->SetGameStart(true);
	vector<CClient*> vecClnt = *CClientMgr::GetInstance()->GetClient();
	for (int i = 0; i < vecClnt.size(); ++i) {
		CClient* pClient = vecClnt[i];
		pClient->SetReady(false);
	}
}
void CNetMgr::OnNotifyCreateObject(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	CREATE_OBJECT_MSG msg;
	recvn(hClntSock, (char*)&msg, sizeof(CREATE_OBJECT_MSG), 0);

	CClient* pClient = CClientMgr::GetInstance()->FindClient(msg.szID);
	CObject*	pObject = NULL;
	VECTOR2 vPos = msg.vCreatePos;
	VECTOR2 vDir = msg.vDir;
	if (pClient)
	{
		switch (msg.eCreateType)
		{
		case E_OBJECT_CREATE_TYPE_BUBBLE:
		{
			CCharacter* pCharacter = pClient->GetCharacter();
			if (pCharacter)
				pObject = CBubble::Create(msg.iID, msg.vCreatePos, pCharacter, pCharacter->GetCharacterInfo()->m_tItemInfo.iPowerCnt);

			break;
		}
		case E_OBJECT_CREATE_TYPE_BANANA:
		{
			CCharacter* pCharacter = pClient->GetCharacter();
			if (pCharacter)
			{

				//VECTOR2 TileCenter;
				//pCharacter->CalcTileCenter(VECTOR2(vPos.x , vPos.y + 20), &TileCenter);
				pCharacter->GetCharacterInfo()->m_tItemInfo.iBananaCnt--;


				CItem* pItem = new CItem(VECTOR2(vPos.x, vPos.y + 10), IMAGE_INFO(L"Item", 40, 40, RGB(0, 255, 0)), (int)E_ITEMTYPE_BANANA);
				pItem->SetOwner(pCharacter);
				pObject = pItem;
			}
			break;
		}
		case E_OBJECT_CREATE_TYPE_MINE:
		{
			CCharacter* pCharacter = pClient->GetCharacter();
			if (pCharacter)
			{
				//VECTOR2 TileCenter;
				//pCharacter->CalcTileCenter(VECTOR2(vPos.x , vPos.y + 20), &TileCenter);
				pCharacter->GetCharacterInfo()->m_tItemInfo.iMineCnt--;


				CItem* pItem = new CItem(VECTOR2(vPos.x, vPos.y + 10), IMAGE_INFO(L"Item", 40, 40, RGB(0, 255, 0)), (int)E_ITEMTYPE_MINE);
				pItem->SetOwner(pCharacter);
				pObject = pItem;
			}
			break;
		}
		case E_OBJECT_CREATE_TYPE_DART:
		{
			CCharacter* pCharacter = pClient->GetCharacter();
			if (pCharacter)
			{
				//VECTOR2 TileCenter;
				//pCharacter->CalcTileCenter(VECTOR2(vPos.x , vPos.y + 20), &TileCenter);
				pCharacter->GetCharacterInfo()->m_tItemInfo.iDartCnt--;


				CDart* pItem = new CDart(VECTOR2(vPos.x, vPos.y + 10), vDir, (int)E_ITEMTYPE_MINE);
				pItem->SetOwner(pCharacter);

				pObject = pItem;
				pObject = pItem;
			}
			break;
		}
		default:
			break;
		}
	}

	if (pObject)
	{
		EnterCriticalSection(&g_cs);
		CObjectMgr::GetInstance()->ServerCreateObject(pObject);
		LeaveCriticalSection(&g_cs);
	}
}
void CNetMgr::OnNotifyChat(LPVOID sock)
{
	char chat[CHAT_LEN];
	int retval = 0;
	SOCKET	hClntSock = *((SOCKET*)sock);
	retval = recvn(hClntSock, (char*)chat, sizeof(chat), 0);
	printf("retval : %d\n", retval);
	//chat[retval] = '\0'; // 채팅의 끝 알려주기 위해


	CChatMgr::GetInstance()->AddChat(chat);
	printf("%s\n", &chat);
}

//---------------예외처리-----------------
void CNetMgr::OnOverlapLogin(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	printf("로그인 중복 로그인 실패\n");
	shutdown(hClntSock, SD_BOTH);

	closesocket(hClntSock);

	MessageBox(g_hWnd, L"중복된 아이디 입니다. 다른 아이디를 이용해 주세요.", L"", NULL);
}
void CNetMgr::OnFailChoiceMap(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	printf("게임 시작 실패 맵을 고르시오\n");
	MessageBox(g_hWnd, L"아직 준비가 덜 된 것 같은데? : 맵 선택", L"", NULL);
}
void CNetMgr::OnFailReady(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	printf("게임 시작 실패 다른 유저 준비가 안됨\n");
	MessageBox(g_hWnd, L"아직 준비가 덜 된 것 같은데? : 레디 상태 확인", L"", NULL);
}


void CNetMgr::OnNotifyRemoveObject(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	REMOVE_OBJECT_MSG msg;
	recvn(hClntSock, (char*)&msg, sizeof(REMOVE_OBJECT_MSG), 0);

	CObject* pObject = CObjectMgr::GetInstance()->FindObject(msg.iID);
	if (pObject)
	{
		//삭제할 데이터 추가
		EnterCriticalSection(&g_cs);
		CObjectMgr::GetInstance()->ServerRemoveObject(pObject);
		LeaveCriticalSection(&g_cs);
	}

}

void CNetMgr::OnNotifyBubbleKick(LPVOID sock)
{
	SOCKET	hClntSock = *((SOCKET*)sock);
	BUBBLE_KICK_MSG msg;
	recvn(hClntSock, (char*)&msg, sizeof(BUBBLE_KICK_MSG), 0);

	int iID = msg.iID;
	CObject* pObject = CObjectMgr::GetInstance()->FindObject(E_OBJ_LIST_TYPE_BUBBLE, iID);
	CBubble* pBubble = dynamic_cast<CBubble*>(pObject);
	if (pBubble)
	{
		//pBubble->GetInfo()->vPos = vPos;
		pBubble->SetKick(true, msg.vDir);
	}
}
void CNetMgr::OnNotifyGameOver(LPVOID sock) {
	SOCKET	hClntSock = *((SOCKET*)sock);
	int winner;
	recvn(hClntSock, (char*)&winner, sizeof(int), 0);
	CGameMgr::GetInstance()->SetGameState((E_GAME_STATE)winner);
}