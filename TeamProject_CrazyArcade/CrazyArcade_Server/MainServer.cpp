#include "Headers.h"
#include "Include.h"
#include "TimeMgr.h"
#include "function.h"
#include "ObjectMgr.h"
#include "Block.h"
#include "Bubble.h"
#include "Protocol.h"
#include<time.h>
//#include "Character_Server.h"

// 전역 변수
CRITICAL_SECTION	g_cs;
vector<USER_INFO*>	g_vecUser;
vector<FUNC>		g_vecFunc;
char				g_mapType = E_MAP_TYPE_END;


queue<DATA_INFO>		g_qDataInfo;

// Update 변수
float				g_fTime = E_MSG_TYPE_END;


// 테스트용
bool				g_IsIngame = false;


DWORD	WINAPI UpdateThread(LPVOID arg);
DWORD	WINAPI RecvThread(LPVOID arg);

void FuncInitialize();//함수 등록
void DisConnectClient(USER_INFO* pUser);

// Request =======================
void OnRequestLogin(LPVOID arg);
void OnRequestCharacterInfo(LPVOID arg);
void OnRequestGameStart(LPVOID arg);
void OnRequestPushReady(LPVOID arg);//
void OnRequestSelMap(LPVOID arg);//
void OnRequestChangeCharacter(LPVOID qSock);//

void OnSucessReady(LPVOID arg);//
void OnRequestCreateObject(LPVOID arg);//
void OnRequestBubbleKick(LPVOID arg);//

void OnRequestChatting(LPVOID arg);//

void OnRequestGameOver(LPVOID arg);
// Notify ===========================
void OnNotifyTime();
void OnNotifyLoadTile(LoadTileMSG);
void OnNotifyLoadBlock(int iID, const TCHAR* szKey, int iBlockType, int iX, int iY, int iWidth, int iHeight, UINT iRGB, int iCnt, int iItemType);
void OnNotifyCharacterInfo();//
void OnNotifyRemoveObject(int iObjType, int iID);//

void LoadTile();
void LoadBlock();
//=============================

void MsgProcess();//메세지 처리
void NewGame();

void NewGame()
{
	CObjectMgr::GetInstance()->Clear();
}

void Release();
void CheckMsg(int headers);

int main()
{
	srand(unsigned(time(NULL)));
	WSADATA			wsa;
	SOCKET			listen_sock;
	SOCKADDR_IN		Serveraddr;
	int				retval;

	InitializeCriticalSection(&g_cs);//임계영역 초기화

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		err_quit("WSAStartup() error!");

	FuncInitialize();

	cout << "IP : " << GetMyIP() << endl;

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	//Bind
	ZeroMemory(&Serveraddr, sizeof(Serveraddr));
	Serveraddr.sin_family = AF_INET;
	Serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	Serveraddr.sin_port = htons(SERVER_PORT);

	retval = bind(listen_sock, (const SOCKADDR*)&Serveraddr, sizeof(Serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	SOCKET			Client_Sock;
	SOCKADDR_IN		Client_Addr;
	int				iAddrLen;

	CTimeMgr::GetInstance()->InitTime();

	// 업데이트 쓰레드 생성
	HANDLE hThread = CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);
	if (hThread)
		CloseHandle(hThread);



	while (1)
	{
		iAddrLen = sizeof(Client_Addr);
		Client_Sock = accept(listen_sock, (SOCKADDR*)&Client_Addr, &iAddrLen);
		if (Client_Sock == INVALID_SOCKET)
			err_display("accept()");

		USER_INFO* pNewUser = new USER_INFO;
		pNewUser->sock = Client_Sock;
		pNewUser->addr = Client_Addr;

		// 연결 끊었을 경우 쓰레드에서 유저 정보 삭제할 경우 문제 발생 우려 
		// 임계영역 처리
		EnterCriticalSection(&g_cs);
		g_vecUser.push_back(pNewUser);
		LeaveCriticalSection(&g_cs);

		//쓰레드 생성
		HANDLE hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)pNewUser, 0, NULL);
		if (hThread)
			CloseHandle(hThread);
		printf("Connected Client IP : %s\tProt : %d\n", inet_ntoa(pNewUser->addr.sin_addr), ntohs(pNewUser->addr.sin_port));
	}
	closesocket(listen_sock);


	Release();

}

// 실제 게임이 돌아가는 Update; 
DWORD	WINAPI UpdateThread(LPVOID arg)
{
	//float g_fPosTime = 0.0f;// 캐릭터 위치 전달
	while (1)
	{
		MsgProcess();//메세지 처리
		if (g_IsIngame == false)
			continue;
		// 위치 및 상태 보내기
		//충돌 체크
		CTimeMgr::GetInstance()->SetTime();//타임 갱신
		g_fTime += 1.0f * CTimeMgr::GetInstance()->GetTime();
		if (g_fTime >= 1.0f)
		{
			g_fTime = 0;
			//1초에 한번씩 게임 시간 전달
			OnNotifyTime();
		}
		//for (size_t i = 0; i < E_OBJ_LIST_END; ++i)
		//{
		//	vector<CObject*>	vecObject = *CObjectMgr::GetInstance()->GetVecObject(i);
		//	for (size_t j = 0; j < vecObject.size(); ++j)
		//	{
		//		if (vecObject[j]->Update() == 1)
		//		{

		//		}
		//	}
		//}
		////// 충돌 체크 후 결과 전송
		//for (size_t i = 0; i < g_vecUser.size(); ++i)
		//{
		//	if (g_vecUser[i]->pCharacter)
		//	{
		//		g_vecUser[i]->pCharacter->Update();
		//	}
		//}
	}
}

DWORD	WINAPI RecvThread(LPVOID arg)
{
	USER_INFO* User = (USER_INFO*)arg;
	int				iHeader;
	int retval;
	while (1)
	{
		//헤더 정보 받기 ex ) E_MSG_TYPE_MOVE 
		retval = recvn(User->sock, (char*)&iHeader, sizeof(iHeader), 0);
		//printf("iHeader : %d \n", iHeader);
		if (retval == 0 || retval == SOCKET_ERROR)
		{
			//연결 끊김
			printf("Disconnected Client\n");
			return 0;
		}
		else if (iHeader < (int)g_vecFunc.size())
		{
			//CheckMsg(iHeader);
			g_vecFunc[iHeader].func(User);	//헤더에 맞는 함수 호출
		}
	}
}
void FuncInitialize()
{
	g_vecFunc.resize(E_MSG_TYPE_END);
	g_vecFunc[E_MSG_TYPE_REQUEST_LOGIN].func = OnRequestLogin;//로그인 요청
	g_vecFunc[E_MSG_TYPE_REQUEST_CHARINFO].func = OnRequestCharacterInfo;
	g_vecFunc[E_MSG_TYPE_REQUEST_GAME_START].func = OnRequestGameStart;
	//
	g_vecFunc[E_MSG_TYPE_REQUEST_PUSH_READY].func = OnRequestPushReady;//준비 상태 변경
	g_vecFunc[E_MSG_TYPE_REQUEST_SEL_MAP].func = OnRequestSelMap;//맵 선택 변경
	g_vecFunc[E_MSG_TYPE_REQUEST_CHANGE_CHARACTER].func = OnRequestChangeCharacter;//캐릭터 변경& 팀변경
	g_vecFunc[E_MSG_TYPE_SUCESS_READY].func = OnSucessReady;
	g_vecFunc[E_MSG_TYPE_REQUEST_GAME_OVER].func = OnRequestGameOver;
	//
	g_vecFunc[E_MSG_TYPE_REQUEST_CREATE_OBJECT].func = OnRequestCreateObject;
	g_vecFunc[E_MSG_TYPE_REQUEST_BUBBLE_KICK].func = OnRequestBubbleKick;

	//
	g_vecFunc[E_MSG_TYPE_REQUEST_CHAT].func = OnRequestChatting;//
}
void DisConnectClient(USER_INFO* pUser)//
{
	auto iter = find_if(g_vecUser.begin(), g_vecUser.end(), [pUser](USER_INFO* User)
	{
		if (User == pUser)
		{
			pUser->Release();
			return 1;
		}
		return 0;
	});
	if (iter != g_vecUser.begin())
	{
		delete *iter;
		g_vecUser.erase(iter);
	}
}
void OnRequestLogin(LPVOID arg)//
{
	USER_INFO*	User = (USER_INFO*)arg;
	LOGIN_MSG LoginMsg;

	int protocol = 0;

	recvn(User->sock, (char*)&LoginMsg, sizeof(LOGIN_MSG), 0);
	strcpy_s(User->info.szID, LoginMsg.ID);

	int count = 0;
	for (int i = 0; i < g_vecUser.size(); ++i) {
		if (strcmp(LoginMsg.ID, g_vecUser[i]->info.szID) == 0)++count;
		if (count > 1) {
			protocol = E_MSG_TYPE_OVERLAP_LOGIN;
			send(User->sock, (char*)&protocol, sizeof(int), 0);
			g_vecUser.pop_back();
			return;

		}//중복에 대한 처리 msg를 보내게 바꿔야함
	}
	if (g_vecUser.size() == 1)//처음 들어오면
	{
		User->info.IsRoomMaster = true;
	}

	// Test 용 임시 위치 팀 적용
	User->info.cStartPos = (char)g_vecUser.size();
	if (g_vecUser.size() % 2 == 0) {
		User->info.cCharacter = E_CHARACTER_TYPE_BAZZI;
		User->info.cTeam = E_TEAM_TYPE_RED;
	}
	else
	{
		User->info.cCharacter = E_CHARACTER_TYPE_DAO;
		User->info.cTeam = E_TEAM_TYPE_BLUE;
	}

	NOTIFY_LOGIN_MSG ClientInfo;
	strcpy_s(ClientInfo.info.szID, User->info.szID);
	ClientInfo.info.IsReady = User->info.IsReady;
	ClientInfo.info.IsRoomMaster = User->info.IsRoomMaster;
	ClientInfo.info.cTeam = User->info.cTeam;
	ClientInfo.info.cCharacter = User->info.cCharacter;
	ClientInfo.info.cStartPos = User->info.cStartPos;

	ClientInfo.imaptype = g_mapType;



	NOTIFY_LOGIN_MSG* pClientInfo = new NOTIFY_LOGIN_MSG;
	*pClientInfo = ClientInfo;

	protocol = E_MSG_TYPE_NOTIFY_LOGIN;
	DATA_INFO tdataInfo;
	tdataInfo.protocol = protocol;
	tdataInfo.pStruct = pClientInfo;
	tdataInfo.sock = &User->sock;

	EnterCriticalSection(&g_cs);
	g_qDataInfo.push(tdataInfo);

	//send(User->sock, (char*)&protocol, sizeof(int), 0);
	//send(User->sock, (char*)&ClientInfo, sizeof(NOTIFY_LOGIN_MSG), 0);


	// 나에게 존재하는 모든클라 정보를 알려준다.
	
	protocol = E_MSG_TYPE_NOTIFY_NEW_CLIENT;
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		strcpy_s(ClientInfo.info.szID, g_vecUser[i]->info.szID);
		ClientInfo.info.cCharacter = g_vecUser[i]->info.cCharacter;
		ClientInfo.info.cTeam = g_vecUser[i]->info.cTeam;
		ClientInfo.info.IsReady = g_vecUser[i]->info.IsReady;
		ClientInfo.info.IsRoomMaster = g_vecUser[i]->info.IsRoomMaster;
		ClientInfo.info.cStartPos = g_vecUser[i]->info.cStartPos;

		CLIENT_INFO* cClientInfo = new CLIENT_INFO;
		*cClientInfo = ClientInfo.info;
		tdataInfo.protocol = protocol;
		tdataInfo.pStruct = cClientInfo;
		tdataInfo.sock = &User->sock;
		g_qDataInfo.push(tdataInfo);

		//send(User->sock, (char*)&protocol, sizeof(int), 0);
		//send(User->sock, (char*)&ClientInfo.info, sizeof(CLIENT_INFO), 0);
	}
	//// 나 추가된것을 나만 빼고 알리자
	strcpy_s(ClientInfo.info.szID, User->info.szID);
	ClientInfo.info.cCharacter = User->info.cCharacter;
	ClientInfo.info.cTeam = User->info.cTeam;
	ClientInfo.info.IsReady = User->info.IsReady;
	ClientInfo.info.IsRoomMaster = User->info.IsRoomMaster;
	ClientInfo.info.cStartPos = User->info.cStartPos;
	for (size_t i = 0; i < g_vecUser.size() - 1; ++i)
	{
		CLIENT_INFO* cClientInfo = new CLIENT_INFO;
		*cClientInfo = ClientInfo.info;

		protocol = E_MSG_TYPE_NOTIFY_NEW_CLIENT;
		tdataInfo.protocol = protocol;
		tdataInfo.pStruct = cClientInfo;
		tdataInfo.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(tdataInfo);

		//send(g_vecUser[i]->sock, (char*)&protocol, sizeof(int), 0);
		//send(g_vecUser[i]->sock, (char*)&ClientInfo.info, sizeof(CLIENT_INFO), 0);
	}
	LeaveCriticalSection(&g_cs);

	//////// 테스트용 지울꺼임
	//if (g_vecUser.size() == g_StartUserCnt)
	//{
	//	// 테스트용
	//	// 게임 시작!!! ID 초기화 및 오브젝트 초기화
	//	NewGame();
	//	// 타일 및 블럭 전달

	//	LoadTile();
	//	LoadBlock();

	//}
}
void OnRequestPushReady(LPVOID arg)//
{
	USER_INFO*   User = ((USER_INFO*)arg);
	User->info.IsReady = !User->info.IsReady;
	ReadyPush READY_USER;
	recvn(User->sock, (char*)&READY_USER, sizeof(ReadyPush), 0);
	User->info.IsReady = READY_USER.IsReady;
	int protocol = E_MSG_TYPE_NOTIFY_PUSH_READY;

	DATA_INFO tdataInfo;
	tdataInfo.protocol = protocol;
	


	EnterCriticalSection(&g_cs);

	for (int i = 0; i < g_vecUser.size(); ++i) {
		ReadyPush*Ready_User = new ReadyPush;
		*Ready_User = READY_USER;
		tdataInfo.pStruct = Ready_User;
		tdataInfo.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(tdataInfo);
		//send(g_vecUser[i]->sock, (char*)&protocol, sizeof(int), 0);
		//send(g_vecUser[i]->sock, (char*)&READY_USER, sizeof(ReadyPush), 0);
	}
	LeaveCriticalSection(&g_cs);
}
void OnRequestSelMap(LPVOID arg)//
{
	USER_INFO*   User = ((USER_INFO*)arg);
	if (User->info.IsRoomMaster) {
		recvn(User->sock, (char*)&g_mapType, sizeof(char), 0);
		int protocol = E_MSG_TYPE_NOTIFY_SEL_MAP;
		int selmap = g_mapType;
		DATA_INFO tdataInfo;
		tdataInfo.protocol = protocol;
		tdataInfo.map = selmap;


		EnterCriticalSection(&g_cs);

		for (int i = 0; i < g_vecUser.size(); ++i) {
			tdataInfo.sock = &g_vecUser[i]->sock;
			g_qDataInfo.push(tdataInfo);
		}
		LeaveCriticalSection(&g_cs);
	}
}
void OnRequestChangeCharacter(LPVOID qSock)//
{
	USER_INFO*   User = ((USER_INFO*)qSock);
	ChangeCharacter Character_info;
	recvn(User->sock, (char*)&Character_info, sizeof(ChangeCharacter), 0);
	User->info.cCharacter = Character_info.cCharacter;
	User->info.cTeam = Character_info.cTeam;
	int protocol = E_MSG_TYPE_NOTIFY_CHANGE_CHARACTER;
	
	DATA_INFO tdataInfo;
	tdataInfo.protocol = protocol;
	User->info.cCharacter = Character_info.cCharacter;
	User->info.cTeam = Character_info.cTeam;
	EnterCriticalSection(&g_cs);

	for (int i = 0; i < g_vecUser.size(); ++i) {
			ChangeCharacter* Character_inf = new ChangeCharacter;
		*Character_inf = Character_info;
		tdataInfo.pStruct = Character_inf;
		tdataInfo.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(tdataInfo);
	}
	LeaveCriticalSection(&g_cs);
}
void OnSucessReady(LPVOID arg)//
{
	static int ReadyOrder = 0;
	USER_INFO*   User = ((USER_INFO*)arg);
	EnterCriticalSection(&g_cs);
	++ReadyOrder;
	if (ReadyOrder == g_vecUser.size()) {
		int protocol = E_MSG_TYPE_NOTIFY_GAME_START;
		g_IsIngame = true;
		for (int i = 0; i < g_vecUser.size(); ++i) {
			g_vecUser[i]->info.IsReady = false;
			DATA_INFO tdataInfo;
			tdataInfo.protocol = protocol;
			tdataInfo.sock = &g_vecUser[i]->sock;
			g_qDataInfo.push(tdataInfo);
		}
		ReadyOrder = 0;
		g_mapType = E_MAP_TYPE_END;

	}
	LeaveCriticalSection(&g_cs);
}
void OnRequestGameOver(LPVOID arg) {
	USER_INFO*	User = (USER_INFO*)arg;

	int winteam;
	recvn(User->sock, (char*)&winteam, sizeof(int), 0);
	int blue_live = 0;
	int red_live = 0;
	char* live_char = new char[g_vecUser.size()];
	for (int i = 0; i < g_vecUser.size(); ++i) {//생존자 확인
		if (g_vecUser[i]->pCharacter->GetInfo()->iState != E_CHARACTER_STATE_DEATH) {
			if (g_vecUser[i]->info.cTeam == E_TEAM_TYPE_BLUE) {
				++blue_live;
			}
			else if (g_vecUser[i]->info.cTeam == E_TEAM_TYPE_RED) {
				++red_live;
			}
		}
	}
	EnterCriticalSection(&g_cs);
	if (blue_live==0||red_live==0) {//생존자 있을때
		int protocol = E_MSG_TYPE_NOTIFY_GAME_OVER;
		if (red_live == 0) {
			winteam = E_GAME_STATE_BLUE_WIN;
		}
		else if (blue_live == 0) {
			winteam = E_GAME_STATE_RED_WIN;
		}
		for (size_t i = 0; i < g_vecUser.size(); ++i)
		{
			DATA_INFO data_Info;
			data_Info.protocol = protocol;
			data_Info.sock = &g_vecUser[i]->sock;
			data_Info.winner = winteam;
			g_qDataInfo.push(data_Info);
		}
	}
	else if (winteam == E_GAME_STATE_TIME_OUT || (blue_live==0&&red_live==0)) {//없거나 시간  종료일때
		int protocol = E_MSG_TYPE_NOTIFY_GAME_OVER;
		winteam = E_GAME_STATE_TIME_OUT;
		for (size_t i = 0; i < g_vecUser.size(); ++i)
		{
			DATA_INFO data_Info;
			data_Info.protocol = protocol;
			data_Info.sock = &g_vecUser[i]->sock;
			data_Info.winner = winteam;
			g_qDataInfo.push(data_Info);
		}
	}
	LeaveCriticalSection(&g_cs);
}

void OnRequestCharacterInfo(LPVOID arg)//
{
	USER_INFO*	User = (USER_INFO*)arg;

	if (User->pCharacter == NULL)
	{
		User->pCharacter = new CCharacter_Server;
	}
	CHAR_INFO_MSG	CharInfo;
	recvn(User->sock, (char*)&CharInfo, sizeof(CHAR_INFO_MSG), 0);

	//	printf("OnRequestCharacterInfo  ID %s vPos x %f %f\n\n", CharInfo.szID, CharInfo.vPos.x, CharInfo.vPos.y);


	//이전 위치 셋팅
	User->pCharacter->GetVecPrevPos()->x = User->pCharacter->GetInfo()->vPos.x;
	User->pCharacter->GetVecPrevPos()->y = User->pCharacter->GetInfo()->vPos.y;

	User->pCharacter->SetInfo(CharInfo);
	// 처음에 초기화
	if (User->pCharacter->GetVecPrevPos()->x == 0
		&& User->pCharacter->GetVecPrevPos()->y == 0)
	{
		User->pCharacter->GetVecPrevPos()->x = User->pCharacter->GetInfo()->vPos.x;
		User->pCharacter->GetVecPrevPos()->y = User->pCharacter->GetInfo()->vPos.y;
	}

	DATA_INFO	data_info;
	int protocol = E_MSG_TYPE_NOTIFY_CHARINFO;
	// 나를 제외한 사람에게 보내기
	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		if (g_vecUser[i] == User)
			continue;
		CHAR_INFO_MSG* pCharInfo = new CHAR_INFO_MSG;
		*pCharInfo = CharInfo;

		data_info.protocol = protocol;
		data_info.pStruct = pCharInfo;
		data_info.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(data_info);
	}
	LeaveCriticalSection(&g_cs);
}

void OnRequestGameStart(LPVOID arg)	//
{
	USER_INFO*	User = (USER_INFO*)arg;

	//테스트용 
	DATA_INFO	data_info;
	int protocol;
	if (User->info.IsRoomMaster == false)
	{
		return;//방장이 아닙니다.
	}
	if (g_mapType == E_MAP_TYPE_END)//맵선택이 안되있으면
	{
		protocol = E_MSG_TYPE_FAIL_GAMESTART_CHOICE_MAP;
		data_info.sock = &User->sock;
		data_info.protocol = protocol;
		EnterCriticalSection(&g_cs);
		g_qDataInfo.push(data_info);
		LeaveCriticalSection(&g_cs);
		//send(User->sock, (char*)&protocol, sizeof(int), 0);
		return;
	}
	for (int i = 0; i < g_vecUser.size(); ++i)
	{
		if (User == g_vecUser[i])//방장을 제외하고
			continue;
		else if (g_vecUser[i]->info.IsReady == false)
		{
			int protocol = E_MSG_TYPE_FAIL_GAMESTART_READY;
			data_info.sock = &User->sock;
			data_info.protocol = protocol;
			EnterCriticalSection(&g_cs);
			g_qDataInfo.push(data_info);
			LeaveCriticalSection(&g_cs);

			//send(User->sock, (char*)&protocol, sizeof(int), 0);
			return;//하나라도 레디 상태가 아니면 종료
		}
	}
	// 게임 시작!!! ID 초기화 및 오브젝트 초기화
	NewGame();
	// 타일 및 블럭 전달
	LoadTile();
	LoadBlock();
}

void OnRequestCreateObject(LPVOID arg)
{
	USER_INFO*	User = (USER_INFO*)arg;

	int protocol = E_MSG_TYPE_NOTIFY_CREATE_OBJECT;
	//오브젝트 생성 서버에서도 추가
	CREATE_OBJECT_MSG	ObjectMsg;

	recvn(User->sock, (char*)&ObjectMsg, sizeof(CREATE_OBJECT_MSG), 0);
	DATA_INFO	data_info;
	//모든 클라이언트에게 전달
	//오브젝트 생성
	switch (ObjectMsg.eCreateType)
	{
	case E_OBJECT_CREATE_TYPE_BUBBLE:
	{
		CObject* pObject = new CBubble();
		pObject->GetPos()->x = ObjectMsg.vCreatePos.x;
		pObject->GetPos()->y = ObjectMsg.vCreatePos.y;

		ObjectMsg.szID;
		CObjectMgr::GetInstance()->AddObject(E_OBJ_LIST_TYPE_BUBBLE, pObject);
		ObjectMsg.iID = pObject->GetID();
	}
	break;
	}

	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		CREATE_OBJECT_MSG* pObjectCreate = new CREATE_OBJECT_MSG;
		*pObjectCreate = ObjectMsg;

		data_info.protocol = protocol;
		data_info.pStruct = pObjectCreate;
		data_info.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(data_info);
	}
	LeaveCriticalSection(&g_cs);
}
void OnRequestBubbleKick(LPVOID arg)
{
	USER_INFO*	User = (USER_INFO*)arg;

	int protocol = E_MSG_TYPE_NOTIFY_BUBBLE_KICK;
	BUBBLE_KICK_MSG msg;
	DATA_INFO	data_info;
	recvn(User->sock, (char*)&msg, sizeof(BUBBLE_KICK_MSG), 0);
	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		BUBBLE_KICK_MSG* pKick = new BUBBLE_KICK_MSG;
		*pKick = msg;

		data_info.protocol = protocol;
		data_info.pStruct = pKick;
		data_info.sock = &g_vecUser[i]->sock;

		g_qDataInfo.push(data_info);
	}
	LeaveCriticalSection(&g_cs);
}



void OnRequestChatting(LPVOID arg)// 채팅 추가
{
	USER_INFO*	User = (USER_INFO*)arg;

	char chat[CHAT_LEN];
	DATA_INFO	data_info;
	int protocol = E_MSG_TYPE_NOTIFY_CHAT;
	data_info.protocol = protocol;
	recvn(User->sock, chat, sizeof(chat), 0);
	//printf("채팅 보낸 클라이언트 : %s, 메시지 : %s\n", User->info.szID, chat);
	memcpy_s(data_info.chat, CHAT_LEN, chat, CHAT_LEN);
	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		data_info.sock = &g_vecUser[i]->sock;

		g_qDataInfo.push(data_info);

	}
	LeaveCriticalSection(&g_cs);
}



void OnNotifyCharacterInfo()
{
	/*for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
	USER_INFO* pUser = g_vecUser[i];
	CHAR_INFO_MSG	CharInfo;
	CharInfo.vPos = pUser->pCharacter->GetInfo()->vPos;
	CharInfo.vDir = pUser->pCharacter->GetInfo()->vDir;
	CharInfo.iState = pUser->pCharacter->GetInfo()->iState;
	CharInfo.iMoveState = pUser->pCharacter->GetInfo()->iMoveState;

	strcpy_s(CharInfo.szID, pUser->pCharacter->GetInfo()->szID);
	int protocol = E_MSG_TYPE_NOTIFY_CHARINFO;
	send(g_vecUser[i]->sock, (char*)&protocol, sizeof(int), 0);
	send(g_vecUser[i]->sock, (char*)&CharInfo, sizeof(CHAR_INFO_MSG), 0);
	}*/
}


void LoadTile()//
{
	HANDLE		hFile = NULL;
	DWORD	dwByte = 0;
	if (g_mapType == E_MAP_TYPE_1)
	{
		hFile = CreateFile(L"./Data/Map1_SaveTile.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (g_mapType == E_MAP_TYPE_2)
	{
		hFile = CreateFile(L"./Data/Map2_SaveTile.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (g_mapType == E_MAP_TYPE_3)
	{
		hFile = CreateFile(L"./Data/Map3_SaveTile.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (hFile == NULL)
	{
		printf("파일 로드 실패\n");
		return;
	}
	LoadTileMSG Tiles;
	for (int i = 0; i < TILE_CNTY; ++i)
	{
		for (int j = 0; j < TILE_CNTX; ++j)
		{
			int iOption = 0;
			int iX = 0;
			int iY = 0;
			ReadFile(hFile, &Tiles.i_AllOption[i][j], sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &Tiles.i_AllX[i][j], sizeof(int), &dwByte, NULL);
			ReadFile(hFile, &Tiles.i_AllY[i][j], sizeof(int), &dwByte, NULL);
			//보내자
		}
	}
	OnNotifyLoadTile(Tiles);
	CloseHandle(hFile);
}
void LoadBlock()//
{
	HANDLE		hFile = NULL;
	DWORD	dwByte = 0;
	E_MAP_TYPE eMapType = (E_MAP_TYPE)g_mapType;
	if (eMapType == E_MAP_TYPE_1)
	{
		hFile = CreateFile(L"./Data/Map1_SaveBlock.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (eMapType == E_MAP_TYPE_2)
	{
		hFile = CreateFile(L"./Data/Map2_SaveBlock.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (eMapType == E_MAP_TYPE_3)
	{
		hFile = CreateFile(L"./Data/Map3_SaveBlock.dat", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if (hFile == NULL)
	{
		puts("블럭 로드 실패");
		return;
	}
	int iCnt = 0;
	ReadFile(hFile, &iCnt, sizeof(int), &dwByte, NULL);//블럭 갯수
	//printf("%d\n", iCnt);
	for (int i = 0; i < iCnt; ++i)
	{
		int iBlockType = 0;
		float fX = 0.f;//위치
		float fY = 0.f;
		TCHAR szKey[MIN_STR];
		int iWidth = 0;
		int iHeight = 0;
		UINT iRGB = 0;

		ReadFile(hFile, &iBlockType, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &fX, sizeof(float), &dwByte, NULL);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, NULL);
		ReadFile(hFile, &szKey, sizeof(TCHAR) * MIN_STR, &dwByte, NULL);
		ReadFile(hFile, &iWidth, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iHeight, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iRGB, sizeof(UINT), &dwByte, NULL);


		//블럭 오브젝트 생성
		CBlock* pObject = new CBlock(iBlockType);
		pObject->GetPos()->x = fX;
		pObject->GetPos()->y = fY;

		//Item넣어주자
		int iItemType = rand() % 2;//아이템 들어있을 확률
		if (iItemType == 0)
		{
			iItemType = rand() % E_ITEMTYPE_MINE;

			if (iItemType == E_ITEMTYPE_MAXPOWER
				|| iItemType == E_ITEMTYPE_MAXSPEED
				|| iItemType == E_ITEMTYPE_KICK)
			{
				iItemType = rand() % E_ITEMTYPE_MINE;//한번더
			}
			if (iItemType == E_ITEMTYPE_KICK) {
				iItemType = E_ITEMTYPE_END;
			}
		}
		else
		{
			iItemType = E_ITEMTYPE_END;
		}
		pObject->SetItemType(iItemType);
		CObjectMgr::GetInstance()->AddObject(pObject->GetObjType(), pObject);
		OnNotifyLoadBlock(pObject->GetID(), szKey, iBlockType, (int)fX, (int)fY, iWidth, iHeight, iRGB, iCnt, iItemType);
	}
	CloseHandle(hFile);
}
//
void OnNotifyLoadBlock(int iID, const TCHAR* szKey, int iBlockType, int iX, int iY, int iWidth, int iHeight, UINT iRGB, int iCnt, int iItemType)
{
	int protocol = E_MSG_TYPE_NOTIFY_LOAD_BLOCK;
	LOADBLOCK_MSG	blockMsg;
	blockMsg.iID = iID;//쓰레긴데?
	lstrcpy(blockMsg.strKey, szKey);
	blockMsg.iBlockType = iBlockType;
	blockMsg.iX = iX;
	blockMsg.iY = iY;
	blockMsg.iWidth = iWidth;
	blockMsg.iHeight = iHeight;
	blockMsg.iRGB = iRGB;
	blockMsg.iCnt = iCnt;
	blockMsg.iItemType = iItemType;
	DATA_INFO	data_info;
	//모든 클라이언트에게 전달

	EnterCriticalSection(&g_cs);
	
	data_info.protocol = protocol;
	
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		LOADBLOCK_MSG* Blocks = new LOADBLOCK_MSG;
		*Blocks = blockMsg;
		data_info.pStruct = Blocks;
		data_info.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(data_info);
	}
	LeaveCriticalSection(&g_cs);
}
//
void OnNotifyLoadTile(LoadTileMSG Tiles)
{
	int protocol = E_MSG_TYPE_NOTIFY_LOAD_TILE;
	LOADTILE_MSG	LoadTilemsg;
	LoadTilemsg = Tiles;
	LoadTilemsg.iMapType = g_mapType;
	DATA_INFO	data_info;
	//모든 클라이언트에게 전달

	EnterCriticalSection(&g_cs);
	data_info.protocol = protocol;

	for (int i = 0; i < g_vecUser.size(); ++i)
	{
		LOADTILE_MSG* Tiles_info = new LOADTILE_MSG;
		*Tiles_info = LoadTilemsg;
		data_info.pStruct = Tiles_info;
		data_info.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(data_info);
	}
	LeaveCriticalSection(&g_cs);
}
//
void OnNotifyTime()
{
	int protocol = E_MSG_TYPE_NOTIFY_TIME;
	DATA_INFO data_info;
	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		data_info.protocol = protocol;
		data_info.sock = &g_vecUser[i]->sock;
		g_qDataInfo.push(data_info);
		//	send(g_vecUser[i]->sock, (char*)&protocol, sizeof(int), 0);
	}
	LeaveCriticalSection(&g_cs);
}
void OnNotifyRemoveObject(int iObjType, int iID)
{
	//객체 삭제
	int protocol = E_MSG_TYPE_NOTIFY_REMOVE_OBJECT;
	DATA_INFO data_info;
	EnterCriticalSection(&g_cs);
	for (size_t i = 0; i < g_vecUser.size(); ++i)
	{
		REMOVE_OBJECT_MSG* pMsg = new REMOVE_OBJECT_MSG;
		pMsg->iID = iID;
		pMsg->iObjListType = iObjType;

		data_info.protocol = protocol;
		data_info.sock = &g_vecUser[i]->sock;
		data_info.pStruct = pMsg;
		g_qDataInfo.push(data_info);
	}
	LeaveCriticalSection(&g_cs);
}
void CheckMsg(int headers)
{
	E_MSG_TYPE eType = (E_MSG_TYPE)headers;
	//switch (eType)
	//{
	//case E_MSG_TYPE_REQUEST_LOGIN:
	//	cout << "E_MSG_TYPE_REQUEST_LOGIN" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_LOGIN:
	//	cout << "E_MSG_TYPE_NOTIFY_LOGIN" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_NEW_CLIENT:
	//	cout << "E_MSG_TYPE_NOTIFY_NEW_CLIENT" << endl;
	//	break;
	//case E_MSG_TYPE_REQUEST_PUSH_READY:
	//	cout << "E_MSG_TYPE_REQUEST_PUSH_READY" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_PUSH_READY:
	//	cout << "E_MSG_TYPE_NOTIFY_PUSH_READY" << endl;
	//	break;
	//case E_MSG_TYPE_REQUEST_SEL_MAP:
	//	cout << "E_MSG_TYPE_REQUEST_SEL_MAP" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_SEL_MAP:
	//	cout << "E_MSG_TYPE_NOTIFY_SEL_MAP" << endl;
	//	break;
	//case E_MSG_TYPE_REQUEST_CHANGE_CHARACTER:
	//	cout << "E_MSG_TYPE_REQUEST_CHANGE_CHARACTER" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_CHANGE_CHARACTER:
	//	cout << "E_MSG_TYPE_NOTIFY_CHANGE_CHARACTER" << endl;
	//	break;
	//case E_MSG_TYPE_REQUEST_CHARINFO:
	//	cout << "E_MSG_TYPE_REQUEST_CHARINFO" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_CHARINFO:
	//	cout << "E_MSG_TYPE_NOTIFY_CHARINFO" << endl;
	//	break;
	//case E_MSG_TYPE_REQUEST_CREATE_OBJECT:
	//	cout << "E_MSG_TYPE_REQUEST_CREATE_OBJECT" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_CREATE_OBJECT:
	//	cout << "E_MSG_TYPE_NOTIFY_CREATE_OBJECT" << endl;
	//	break;
	//case E_MSG_TYPE_SUCESS_READY:
	//	cout << "E_MSG_TYPE_SUCESS_READY" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_GAME_START:
	//	cout << "E_MSG_TYPE_NOTIFY_GAME_START" << endl;
	//	break;
	//case E_MSG_TYPE_OVERLAP_LOGIN:
	//	cout << "E_MSG_TYPE_OVERLAP_LOGIN" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_TIME:
	//	cout << "E_MSG_TYPE_NOTIFY_TIME" << endl;
	//	break;
	//case E_MSG_TYPE_REQUEST_GAME_START:
	//	cout << "E_MSG_TYPE_REQUEST_GAME_START" << endl;
	//	break;
	//case E_MSG_TYPE_FAIL_GAMESTART_CHOICE_MAP:
	//	cout << "E_MSG_TYPE_FAIL_GAMESTART_CHOICE_MAP" << endl;
	//	break;
	//case E_MSG_TYPE_FAIL_GAMESTART_READY:
	//	cout << "E_MSG_TYPE_FAIL_GAMESTART_READY" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_LOAD_TILE:
	//	cout << "E_MSG_TYPE_NOTIFY_LOAD_TILE" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_LOAD_BLOCK:
	//	cout << "E_MSG_TYPE_NOTIFY_LOAD_BLOCK" << endl;
	//	break;
	//case E_MSG_TYPE_NOTIFY_CHAT: //
	//	cout << "E_MSG_TYPE_NOTIFY_CHAT" << endl;
	//case E_MSG_TYPE_REQUEST_CHAT: //
	//	cout << "E_MSG_TYPE_REQUEST_CHAT" << endl;
	//case E_MSG_TYPE_END:
	//	break;
	//default:
	//	break;
	//}
}
void Release()
{
	//모든 클라이 언트 소켓 제거
	for (USER_INFO* user : g_vecUser)
	{
		user->Release();
		delete user;
	}
	g_vecUser.clear();
	CTimeMgr::GetInstance()->DestroyInstance();
	CObjectMgr::GetInstance()->DestroyInstance();

	WSACleanup();
}
void MsgProcess()
{
	EnterCriticalSection(&g_cs);
	if (g_qDataInfo.empty() == false)
	{
		DATA_INFO datainfo = g_qDataInfo.front();
		switch (datainfo.protocol)
		{
		case E_MSG_TYPE_NOTIFY_LOGIN:
		{
			NOTIFY_LOGIN_MSG* msg = (NOTIFY_LOGIN_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(NOTIFY_LOGIN_MSG), 0);
				delete msg;
			}

			break;
		}
		case E_MSG_TYPE_NOTIFY_NEW_CLIENT:
		{
			CLIENT_INFO* msg = (CLIENT_INFO*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(CLIENT_INFO), 0);
				delete msg;
			}
			break;

		}
		//case
		case E_MSG_TYPE_NOTIFY_CHARINFO:
		{
			CHAR_INFO_MSG* msg = (CHAR_INFO_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(CHAR_INFO_MSG), 0);
				delete msg;
			}
			break;
		}
		case E_MSG_TYPE_NOTIFY_LOAD_TILE:
		{
			LOADTILE_MSG* msg = (LOADTILE_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(LOADTILE_MSG), 0);
				delete msg;
			}
		}
		break;
		case E_MSG_TYPE_NOTIFY_LOAD_BLOCK:
		{
			LOADBLOCK_MSG* msg = (LOADBLOCK_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(LOADBLOCK_MSG), 0);
				delete msg;
			}
		}
		break;

		case E_MSG_TYPE_NOTIFY_TIME:
		{
			if (datainfo.sock)
			{
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
			}
		}
		break;
		case E_MSG_TYPE_NOTIFY_CREATE_OBJECT:
		{
			CREATE_OBJECT_MSG* msg = (CREATE_OBJECT_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				//CObject* pObject = NULL;
				////오브젝트 생성
				//switch (msg->eCreateType)
				//{
				//case E_OBJECT_CREATE_TYPE_BUBBLE:
				//	pObject = new CBubble;
				//	pObject->GetPos()->x = msg->vCreatePos.x;
				//	pObject->GetPos()->y = msg->vCreatePos.y;
				//	CObjectMgr::GetInstance()->AddObject(E_OBJ_LIST_TYPE_BUBBLE, pObject);
				//	msg->iID = pObject->GetID();
				//	break;
				//default:
				//	pObject = new CObject;
				//	pObject->GetPos()->x = msg->vCreatePos.x;
				//	pObject->GetPos()->y = msg->vCreatePos.y;
				//	CObjectMgr::GetInstance()->AddObject(msg->eCreateType, pObject);
				//	msg->iID = pObject->GetID();
				//	break;
				//}

				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(CREATE_OBJECT_MSG), 0);
				delete msg;
			}
		}
		break;
		case E_MSG_TYPE_NOTIFY_REMOVE_OBJECT:
		{
			REMOVE_OBJECT_MSG* msg = (REMOVE_OBJECT_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				if (CObjectMgr::GetInstance()->RemoveObject(*msg) == true)
				{
					send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
					send(*datainfo.sock, (char*)msg, sizeof(REMOVE_OBJECT_MSG), 0);
				}
				delete msg;
			}
			break;
		}
		case E_MSG_TYPE_NOTIFY_BUBBLE_KICK:
		{
			BUBBLE_KICK_MSG* msg = (BUBBLE_KICK_MSG*)datainfo.pStruct;
			if (msg && datainfo.sock)
			{
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(BUBBLE_KICK_MSG), 0);
				delete msg;
			}
			break;
		}
		case E_MSG_TYPE_NOTIFY_PUSH_READY: {
			ReadyPush* msg = (ReadyPush*)datainfo.pStruct;
			if (msg&&datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(ReadyPush), 0);
				delete msg;

			}
		}
										   break;

		case E_MSG_TYPE_NOTIFY_SEL_MAP: {
			int selmap = datainfo.map;
			if (datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)&selmap, sizeof(int), 0);
			}
		}
										break;

		case E_MSG_TYPE_NOTIFY_CHANGE_CHARACTER: {
			ChangeCharacter* msg = (ChangeCharacter*)datainfo.pStruct;
			if (msg&&datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)msg, sizeof(ChangeCharacter), 0);
				delete msg;
			}
		}
												 break;
		case E_MSG_TYPE_NOTIFY_GAME_START: {
			if (datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
			}
		}
									  break;
		case E_MSG_TYPE_OVERLAP_LOGIN: {
			if (datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
			}}
									   break;
		case E_MSG_TYPE_FAIL_GAMESTART_CHOICE_MAP: {
			if (datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
			}
		}
												   break;
		case E_MSG_TYPE_FAIL_GAMESTART_READY: {
			if (datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
			}
		}
											  break;
		case E_MSG_TYPE_NOTIFY_CHAT: {//

			if (datainfo.sock) {
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)&datainfo.chat, CHAT_LEN, 0);
			//	printf("send한 메시지 : %s\n", datainfo.chat);

			}
		}
									 break;
		case E_MSG_TYPE_NOTIFY_GAME_OVER:
		{
			if (datainfo.sock) {
				g_IsIngame = false;
				send(*datainfo.sock, (char*)&datainfo.protocol, sizeof(int), 0);
				send(*datainfo.sock, (char*)&datainfo.winner, sizeof(int), 0);
			}
		}
		break;
		default:
			break;
		}
		g_qDataInfo.pop();
	}
	LeaveCriticalSection(&g_cs);

}