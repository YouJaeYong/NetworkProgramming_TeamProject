#include "stdafx.h"
#include "Client.h"


CClient::CClient(char* szID)
{
	strcpy_s(m_tClientInfo.szID, szID);
}


CClient::~CClient()
{
}

CCharacter* CClient::GetCharacter(void)
{
	return m_pCharacter;
}

void CClient::SetCharacter(CCharacter* _pCharacter)
{
	m_pCharacter = _pCharacter;
	if (m_pCharacter)
	{
		if (m_tClientInfo.cCharacter == E_CHARACTER_TYPE_BAZZI)
		{
			//m_pCharacter->SetTeam(E_TEAM_TYPE_RED);
		}
		else if (m_tClientInfo.cCharacter == E_CHARACTER_TYPE_DAO)
		{
			//m_pCharacter->SetTeam(E_TEAM_TYPE_BLUE);
		}
	}
}

