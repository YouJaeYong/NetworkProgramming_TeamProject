#include "stdafx.h"
#include "Dao.h"
#include "GameMgr.h"

CDao::CDao(const char* szID, int iStartingIndex, IMAGE_INFO _ImageInfo)
	: CCharacter(szID, iStartingIndex, _ImageInfo)
{
	m_eCharacterType = E_CHARACTER_TYPE_DAO;
}


CDao::~CDao()
{
}

void CDao::Initialize(void)
{
}

void CDao::Update(void)
{
	if (m_IsDestory)
		return;
	CCharacter::Update();
	ChangeState();
}

void CDao::Render(void)
{
	CObject::Render();
}

CDao * CDao::Create(const char* szID, int iStartingIndex)
{
	CDao* pDao = new CDao(szID, iStartingIndex, IMAGE_INFO(L"Character_Dao", 70, 70, RGB(0, 255, 0)));
	pDao->Initialize();

	return pDao;
}

void CDao::ChangeState(void)
{
	if (m_ePrevCharacterState != m_eCharacterState)
	{
		switch (m_eCharacterState)
		{
		case E_CHARACTER_STATE_STAND_UP:
			m_tFrame = FRAME(0, m_eCharacterState, 5, 0, 0.2f);
			break;
		case E_CHARACTER_STATE_STAND_DOWN:
			m_tFrame = FRAME(0, m_eCharacterState, 5, 0, 0.2f);
			break;
		case E_CHARACTER_STATE_STAND_RIGHT:
			m_tFrame = FRAME(0, m_eCharacterState, 5, 0, 0.2f);
			break;
		case E_CHARACTER_STATE_STAND_LEFT:
			m_tFrame = FRAME(0, m_eCharacterState, 5, 0, 0.2f);
			break;
		case E_CHARACTER_STATE_DROWNING:
			m_tFrame = FRAME(0, m_eCharacterState, 4, 0, 2.f);
			break;
		case E_CHARACTER_STATE_REVIVE:
			m_tFrame = FRAME(0, m_eCharacterState, 5, 0, 0.1f);
			break;
		case E_CHARACTER_STATE_DEATH:
			m_tFrame = FRAME(0, m_eCharacterState, 6, 0, 0.2f);
			break;
		}
		//상태 변경  서버에게 알리자
		m_ePrevCharacterState = m_eCharacterState;
	}
}
