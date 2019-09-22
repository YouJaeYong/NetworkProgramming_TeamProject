#pragma once
#include "Headers.h"
#include "Enum.h"
#include "Math.h"

#define SERVER_PORT		8000
#define BUF_SIZE		1024
#define ID_LEN			12

#define CHAT_LEN		128 

enum E_MSG_TYPE
{

	E_MSG_TYPE_REQUEST_LOGIN,
	E_MSG_TYPE_NOTIFY_LOGIN,
	E_MSG_TYPE_NOTIFY_NEW_CLIENT,

	E_MSG_TYPE_REQUEST_PUSH_READY,
	E_MSG_TYPE_NOTIFY_PUSH_READY,
	E_MSG_TYPE_REQUEST_SEL_MAP,
	E_MSG_TYPE_NOTIFY_SEL_MAP,

	E_MSG_TYPE_REQUEST_CHANGE_CHARACTER,
	E_MSG_TYPE_NOTIFY_CHANGE_CHARACTER,
	E_MSG_TYPE_REQUEST_CHARINFO,
	E_MSG_TYPE_NOTIFY_CHARINFO,

	E_MSG_TYPE_REQUEST_CREATE_OBJECT,
	E_MSG_TYPE_NOTIFY_CREATE_OBJECT,

	E_MSG_TYPE_REQUEST_REMOVE_OBJECT,
	E_MSG_TYPE_NOTIFY_REMOVE_OBJECT,


	E_MSG_TYPE_REQUEST_BUBBLE_KICK,
	E_MSG_TYPE_NOTIFY_BUBBLE_KICK,

	E_MSG_TYPE_NOTIFY_TIME,
	E_MSG_TYPE_REQUEST_GAME_START,

	E_MSG_TYPE_NOTIFY_LOAD_TILE,
	E_MSG_TYPE_NOTIFY_LOAD_BLOCK,
	//
	E_MSG_TYPE_SUCESS_READY,
	E_MSG_TYPE_NOTIFY_GAME_START,
	//예외처리
	E_MSG_TYPE_OVERLAP_LOGIN,
	E_MSG_TYPE_FAIL_GAMESTART_CHOICE_MAP,
	E_MSG_TYPE_FAIL_GAMESTART_READY,
	//채팅
	E_MSG_TYPE_REQUEST_CHAT,
	E_MSG_TYPE_NOTIFY_CHAT,
	//게임 종료!
	E_MSG_TYPE_REQUEST_GAME_OVER,
	E_MSG_TYPE_NOTIFY_GAME_OVER,

	E_MSG_TYPE_REQUEST_REPLAY,
	E_MSG_TYPE_NOTIFY_REPLAY,
	E_MSG_TYPE_END
};

//공용 구조체
typedef struct LoginMSG
{
	char	ID[ID_LEN];
}LOGIN_MSG;



typedef struct ReadyPush {
	char	ID[ID_LEN];
	bool IsReady;
}READY_PUSH;
typedef struct ChangeCharacter {
	char	ID[ID_LEN];
	char cTeam;
	char cCharacter;
}CHANGE_CHARACTER;
typedef struct Char_Info
{
	char					szID[ID_LEN];
	VECTOR2					vPos;
	VECTOR2					vDir;
	int						iState;
	int						iMoveState=0;
	char					cKey;
	bool					IsVisible;
	void operator = (const Char_Info& rhs)
	{
		strcpy_s(szID, rhs.szID);
		vPos = rhs.vPos;
		vDir = rhs.vDir;
		iState = rhs.iState;
		iMoveState = rhs.iMoveState;
		cKey = rhs.cKey;
		IsVisible = rhs.IsVisible;
	}
}CHAR_INFO_MSG;
typedef struct Clientinfo
{
	char	szID[ID_LEN];
	bool	IsRoomMaster=false;
	bool	IsReady;
	char	cTeam;
	char	cCharacter;
	char	cStartPos;
	Clientinfo()
	{
		memset(szID, 0, sizeof(szID));
		IsRoomMaster = false;
		IsReady = false;
		cTeam = 0;
		cStartPos = 0;
		cCharacter = E_CHARACTER_TYPE_DAO;
	}
	void operator = (Clientinfo& rhs)
	{
		strcpy_s(szID, rhs.szID);
		IsRoomMaster = rhs.IsRoomMaster;
		IsReady = rhs.IsReady;
		cTeam = rhs.cTeam;
		cCharacter = rhs.cCharacter;
		cStartPos = rhs.cStartPos;
	}
}CLIENT_INFO;

//데이터 정보 하나의 흐름에서 처리하기 위해서 메세지 꼬이는거 막기위해
typedef struct dataInfo
{
	int protocol;//프로토콜 정보
	void* pStruct;//구조체 정보
	SOCKET*	sock;//보낼 소캣 정보
	char map; //map은 따로 구조체가 없다...
	char chat[CHAT_LEN];
	int winner;//
	dataInfo()
	{
		pStruct = NULL;
		sock = NULL;
	}
}DATA_INFO;

typedef struct NotifySuccessLogin
{
	CLIENT_INFO				info;
	int					imaptype;

	void operator = (NotifySuccessLogin& rhs)
	{
		info = rhs.info;
		imaptype = rhs.imaptype;
	}
}NOTIFY_LOGIN_MSG;// 나의 대한 정보 설정해주고 어떤 맵인지 확인하기 위해서


typedef struct LoadTileMSG
{
	int iMapType;
	int i_AllOption[13][15];
	int i_AllX[13][15];
	int i_AllY[13][15];

}LOADTILE_MSG;


typedef struct LoadBlockMSG
{
	int	iID;
	TCHAR strKey[64];
	int iBlockType;
	int iX;
	int iY;
	int iWidth;
	int iHeight;
	int iRGB;
	int iCnt;
	int iItemType;
	void operator = (LoadBlockMSG& rhs)
	{
		lstrcpy(strKey, rhs.strKey);
		iID = rhs.iID;
		iBlockType = rhs.iBlockType;
		iX = rhs.iX;
		iY = rhs.iY;
		iWidth = rhs.iWidth;
		iHeight = rhs.iHeight;
		iRGB = rhs.iRGB;
		iCnt = rhs.iCnt;
		iItemType = rhs.iItemType;
	}
}LOADBLOCK_MSG;

typedef struct Create_Object_Msg
{
	char	szID[ID_LEN];
	int		iID;
	VECTOR2	vCreatePos;
	VECTOR2	vDir;
	E_OBJECT_CREATE_TYPE eCreateType;
	void operator = (Create_Object_Msg& rhs)
	{
		strcpy_s(szID, rhs.szID);
		vCreatePos = rhs.vCreatePos;
		iID = rhs.iID;
		vDir = rhs.vDir;
		eCreateType = rhs.eCreateType;

	}

}CREATE_OBJECT_MSG;

typedef struct Remove_Object_Msg
{
	int iObjListType;
	int iID;
}REMOVE_OBJECT_MSG;

typedef struct BubbleKickMsg
{
	char		szID[ID_LEN];
	int			iID;
	VECTOR2		vPos;
	VECTOR2		vDir;
	void operator = (BubbleKickMsg& rhs)
	{
		strcpy_s(szID, rhs.szID);
		vPos = rhs.vPos;
		iID = rhs.iID;
		vDir = rhs.vDir;
	}
}BUBBLE_KICK_MSG;

