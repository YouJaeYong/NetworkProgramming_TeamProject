#include "Character_Server.h"
#include "Include.h"
#include "CollisionMgr.h"


CCharacter_Server::CCharacter_Server()
{
	m_rcRectSize.left = long(-TILECX / 2);
	m_rcRectSize.right = long(+TILECX / 2);

	m_rcRectSize.top = long(-TILECY / 4);
	m_rcRectSize.bottom = long(+TILECY / 4);
}


CCharacter_Server::~CCharacter_Server()
{
}

void CCharacter_Server::Update()
{
	CalcRect();
}

void CCharacter_Server::CalcRect(void)
{
	m_rcRect.left = m_tInfo.vPos.x + m_rcRectSize.left;
	m_rcRect.right = m_tInfo.vPos.x + m_rcRectSize.right;

	m_rcRect.top = m_tInfo.vPos.y + m_rcRectSize.top;
	m_rcRect.bottom = m_tInfo.vPos.y + m_rcRectSize.bottom;
}
