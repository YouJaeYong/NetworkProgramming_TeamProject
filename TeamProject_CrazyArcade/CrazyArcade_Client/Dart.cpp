#include "stdafx.h"
#include "Dart.h"
#include "CollisionMgr.h"
#include "TimeMgr.h"
//#include "SoundMgr.h"
CDart::CDart(VECTOR2 vPos, VECTOR2  vDir, int iItemType)
	: CItem(vPos, IMAGE_INFO(), iItemType)
{
	m_tImageInfo = IMAGE_INFO(L"Dart", 40, 40, RGB(255, 0, 255));
	m_eRenderType = E_RENDER_TYPE_SORT_OBJECT;
	m_tInfo.m_vDir = vDir;
	m_fSpeed = 1000;
	Initialize();

}


CDart::~CDart()
{
}

void CDart::Initialize(void)
{
	//	CSoundMgr::GetInstance()->PlayEnviroment(L"SFX_Item_Dart.wav", g_fVolume);

	CalcRect();
	m_tFrame = FRAME(m_eItemType, 0, 0, 0, 0);

	m_tInfo.m_vDir.Nomalize();

	if (m_tInfo.m_vDir == VECTOR2(0.f, -1.f))
	{
		m_tFrame.m_iCurFrame = 0;
	}
	else if (m_tInfo.m_vDir == VECTOR2(0.f, 1.f))
	{
		m_tFrame.m_iCurFrame = 1;
	}
	else if (m_tInfo.m_vDir == VECTOR2(1.f, 0.f))
	{
		m_tFrame.m_iCurFrame = 2;
	}
	else if (m_tInfo.m_vDir == VECTOR2(-1.f, 0.f))
	{
		m_tFrame.m_iCurFrame = 3;
	}
}

void CDart::Update(void)
{
	if (m_IsDestory)return;

	CalcRect();

	m_tInfo.vPos += m_tInfo.m_vDir * m_fSpeed * GETTIME;

	RECT rcScreen;
	rcScreen.left = long(20);
	rcScreen.right = long(630);

	rcScreen.top = long(30);
	rcScreen.bottom = long(560);

	POINT pt;
	pt.x = (long)m_tInfo.vPos.x;
	pt.y = (long)m_tInfo.vPos.y;
	if (PtInRect(&rcScreen, pt) == false)
	{
		SetDestroy();
	}
	if (CCollisionMgr::GetInstance()->CheckObjectToBlock(this, m_tInfo.vPos))
	{
		SetDestroy();
		return;
	}

}

void CDart::Render(void)
{
	CObject::Render();
}

void CDart::CalcRect()
{
	m_tInfo.rcRect.left = long(m_tInfo.vPos.x - TILECX / 4);
	m_tInfo.rcRect.right = long(m_tInfo.vPos.x + TILECX / 4);
	m_tInfo.rcRect.top = long(m_tInfo.vPos.y - TILECY / 4);
	m_tInfo.rcRect.bottom = long(m_tInfo.vPos.y + TILECY / 4);
}
