#pragma once
#include "Include.h"
#include "NetInclude.h"

class CCharacter;
class CClient
{
public:
	CClient(char* szID);
	~CClient();

public:
	CLIENT_INFO* GetClientInfo(void) { return &m_tClientInfo; }
	const char* GetID(void) { return m_tClientInfo.szID; }
	CCharacter* GetCharacter(void);
	void SetCharacter(CCharacter* _pCharacter);


	//Çö¿ì
	void SetCharacter(char _pCharacter) {
		m_tClientInfo.cCharacter = _pCharacter;
		m_tClientInfo.cTeam = _pCharacter;
		/*if (_pCharacter == 'D') {
			delete m_pCharacter;
		}*/
	}
	void SetReady(bool IsReady) {
		m_tClientInfo.IsReady = IsReady;
	}
	void GameEnd(bool value) {
		m_IsGameEnd = value;
	}
	bool IsGameEnd() {
		return m_IsGameEnd;
	}
private:
	CLIENT_INFO				m_tClientInfo;
	CCharacter*				m_pCharacter;
	bool					m_IsGameEnd=false;
};
