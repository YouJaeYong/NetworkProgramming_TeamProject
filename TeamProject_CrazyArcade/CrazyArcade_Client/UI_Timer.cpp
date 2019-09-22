#include "stdafx.h"
#include "UI_Timer.h"
#include "TimeMgr.h"
#include "GameMgr.h"

CUI_Timer::CUI_Timer(int iX, int iY, float fStartTime, float fLifeTime, IMAGE_INFO _tImageInfo, E_UI_LEVEL _UILevel, bool _Visible)
	:CUI(iX, iY, _tImageInfo, _UILevel, _Visible)
{
	m_fLifeTime = fLifeTime;
	m_fStartTime = fStartTime;

	m_fCurStartTime = 0.f;
	m_fCurLifeTime = 0.f;
	CGameMgr::GetInstance()->key_start = false;
}

CUI_Timer::~CUI_Timer()
{
}

void CUI_Timer::Update(void)
{
	m_fCurStartTime += GETTIME;

	if (m_fCurStartTime < m_fStartTime)return;


	m_fCurLifeTime += GETTIME;

	if (m_fCurLifeTime > m_fLifeTime)//»èÁ¦
	{
		CGameMgr::GetInstance()->key_start = true;
		m_IsDestroy = true;
		return;
	}
	CUI::Update();
}

void CUI_Timer::Render(void)
{
	if (m_fCurStartTime < m_fStartTime || m_IsDestroy == true)return;

	CUI::Render();

}
