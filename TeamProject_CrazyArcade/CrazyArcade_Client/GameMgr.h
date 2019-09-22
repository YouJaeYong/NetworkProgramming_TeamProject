#pragma once
#include "Include.h"

enum E_GAME_STATE
{
	E_GAME_STATE_RPOGRESS,//진행중
	E_GAME_STATE_BLUE_WIN,
	E_GAME_STATE_RED_WIN,
	E_GAME_STATE_TIME_OUT,//시간 초과
	E_GAME_STATE_END
};



class CGameMgr
{
	DECLARE_SINGLETON(CGameMgr)
private:
	CGameMgr();
	~CGameMgr();
public:
	void Update(void);
	bool IsCollBoxRender(void) { return m_IsCollBoxRender; }
	//	void DrawRect(CObj* pObj);

	void SetGameStart(bool _IsStart);
	void SetMapType(int _eMap) { m_eMapType = (E_MAP_TYPE)_eMap; }
	E_MAP_TYPE GetMapType(void) { return m_eMapType; }
	int GetTime(void) { return m_iCurSec; }
	void SetTime() { --m_iCurSec; }

public:
	int* GetMinute(void) { return &m_iMinute; }
	int* GetFirst_sece(void) { return &m_iFirst_sec; }
	int* GetSecond_sec(void) { return &m_iSecond_sec; }

	void SetGameState(E_GAME_STATE _eState);
	E_GAME_STATE GetGameState(void) { return m_eGameState; }
	
	void NewGame(void);

	bool DirKeyInput(void);
private:
	void Controller(void);
	void GameStart(void);
	void LobbyControll(void);
	void IngameControll(void);
	void CharacterControll(void);//캐릭터 조작
	void CalcTime(void);
	bool GameResult(void);
public:
	bool key_start;
//	static void GameOverMsg(void);
private:
	bool			m_IsCollBoxRender;
	bool			m_IsGameStart;
	E_MAP_TYPE		m_eMapType;

	int				m_iCurSec;

	int			m_iMinute;
	int			m_iFirst_sec;//첫번째 십의 자리 초
	int			m_iSecond_sec;//첫번째 십의 자리 초

	E_GAME_STATE	m_eGameState;
};

